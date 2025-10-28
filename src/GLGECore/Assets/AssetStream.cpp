/**
 * @file AssetStream.cpp
 * @author DM8AT
 * @brief implement the asset streaming system
 * @version 0.1
 * @date 2025-10-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//add the asset streaming header
#include "AssetStream.h"
//add the asset manager system
#include "AssetManager.h"
//for memcpy
#include <cstring>
//for ceil
#include <cmath>

//implement the (now fully defined) asset job registry registry
AssetJobRegistry AssetJobRegistry::REGISTRY = AssetJobRegistry();

void AssetJobRegistry::initialize() noexcept
{
    //this runs before the program starts, so no locking is required

    //pre-allocate some room for the vectors
    m_jobs.reserve(128);
    m_freeList.reserve(m_jobs.capacity());

    //create all the threads
    m_threadPool.resize(std::ceil(std::thread::hardware_concurrency() * 0.33333f));
    for (auto& t : m_threadPool) 
    {t = std::thread(worker_ThreadFunction);}
}

void AssetJobRegistry::cleanup() noexcept
{
    //stop the worker threads
    m_running.store(false, std::memory_order::relaxed);
    m_jobAvailable.notify_all();

    //join back all free threads
    for (auto& t : m_threadPool)
    {t.join();}

    //no more threads are running -> no locking required
    m_jobMutex.unlock();
    m_queueMutex.unlock();

    //clean up all vectors
    m_jobs.clear();
    m_freeList.clear();
}

AssetJobHandle AssetJobRegistry::enqueue(const AssetAsyncLoadJob& job) noexcept {
    //thread safety - the vector MAY resize
    std::unique_lock lock(m_jobMutex);

    //store the index to add
    uint32_t idx = 0;
    //get the first free index and enqueue the job at the index
    if (!m_freeList.empty()) {
        //the free list is not empty. Just use the first known free slot
        idx = m_freeList.back();
        m_freeList.pop_back();
        //store the new entry
        m_jobs[idx].canceled.store(false, std::memory_order::relaxed);
        m_jobs[idx].job = job;
        m_jobs[idx].state.store(AssetJobRegistryEntry::State::UNINITIALIZED, std::memory_order::relaxed);
        //no need to update the version - the version is updated when the asset was deleted (to invalidate the old handles)
    } else {
        //the free list is empty. Just create a new element in the vector to use. 
        idx = m_jobs.size();
        m_jobs.emplace_back(job);
    }

    //create the job handle
    AssetJobHandle handle{
        .index = idx,
        .version = m_jobs[idx].version.load(std::memory_order::relaxed),
        .priority = m_jobs[idx].job.priority
    };
    //queue the job
    {
        std::unique_lock qLock(m_queueMutex);
        m_jobQueue.push(std::move(handle));
    }

    //notify a single worker thread
    m_jobAvailable.notify_one();

    //return the handle
    return handle;
}

void AssetJobRegistry::popJob(const AssetJobHandle& job) noexcept
{
    //working on asset job vector - so lock the mutex
    std::unique_lock lock(m_jobMutex);
    
    //sanity check the handle
    if (job.index >= m_jobs.size()) {return;}
    if (job.version != m_jobs[job.index].version.load(std::memory_order::acquire)) {return;}

    //handle is sane - delete the asset job
    m_jobs[job.index].version.fetch_add(1, std::memory_order::acq_rel);
    //also make sure to add the index to the free list
    //a new queuing won't happen while the lock is still active so cleanup can start
    m_freeList.emplace_back(job.index);
}

bool AssetJobRegistry::isValid(const AssetJobHandle& handle) noexcept
{
    //make sure the read is safe
    std::shared_lock lock(m_jobMutex);
    //check if the handle points to the current asset version
    return (handle.index < m_jobs.size()) ? (m_jobs[handle.index].version.load(std::memory_order::acquire) == handle.version) : false;
}

bool AssetJobRegistry::cancelJob(const AssetJobHandle& handle) noexcept
{
    //make sure the read / write is safe (only writing to atomic, so shared is enough)
    std::shared_lock lock(m_jobMutex);
    //sanity check the handle
    if (handle.index >= m_jobs.size()) {return false;}
    if (handle.version != m_jobs[handle.index].version.load(std::memory_order::acquire)) {return false;}
    //actually cancel the job
    m_jobs[handle.index].cancel();
    //success
    return true;
}

void AssetJobRegistry::worker_ThreadFunction() noexcept
{
    //run while the asset registry is active
    while (m_running.load(std::memory_order::acquire))
    {
        //store the handle of the current job
        AssetJobHandle handle;

        {
            //make sure the queue is locked while getting data from it
            std::unique_lock lock(m_queueMutex);
            //wait for new work
            m_jobAvailable.wait(lock, []{return (!m_jobQueue.empty() || !m_running.load(std::memory_order::acquire));});

            //check if the asset registry was stopped
            if (!m_running.load(std::memory_order::relaxed)) {return;}

            //get the job from the queue
            handle = m_jobQueue.top();
            m_jobQueue.pop();
        }

        //lock the job system SHARED to safely read (no re-sizing will happen)
        {
            std::shared_lock lock(m_jobMutex);

            //sanity check the handle
            if (isValid_Unsafe(handle))
            {
                //only continue if the handle is actually valid

                //get the actual job to execute
                AssetJobRegistryEntry& entry = m_jobs[handle.index];

                //job is being worked on
                entry.state.store(AssetJobRegistryEntry::State::RUNNING, std::memory_order::release);

                //store if the job was successful
                bool success = true;

                //check if the loading was canceled
                if (entry.isCanceled()) {
                    popJob(handle);
                    continue;
                }

                //make sure no unexpected movement happens
                {
                    std::shared_lock lock(AssetManager::m_assetMutex);
                    //call the custom construction function
                    if (AssetManager::isValid(entry.job.handle)) 
                    {AssetManager::m_assets[entry.job.handle.index].ptr->load();}
                    //check success
                    success = AssetManager::m_assets[entry.job.handle.index].ptr->getLoadState() == ASSET_STATE_LOADED;
                }

                //job done
                entry.state.store(AssetJobRegistryEntry::State::FINISHED, std::memory_order::release);

                //if a callback exists, call it
                if (entry.job.callback) 
                {(*entry.job.callback)(entry.job.handle, success);}
            }
        }

        //clean up
        popJob(handle);
    }
}