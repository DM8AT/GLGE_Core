/**
 * @file AssetStream.h
 * @author DM8AT
 * @brief define structures that are used to stream in assets from disk
 * @version 0.1
 * @date 2025-10-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_ASSETS_ASSET_STREAM_
#define _GLGE_CORE_ASSETS_ASSET_STREAM_

//everything is only important for C++ since the jobs are all handled by the asset manager
#if __cplusplus

//add assets to interface with assets correctly
#include "Asset.h"
//add strings for path storage
#include "../../GLGE_BG/CBinding/String.h"

//use atomics for thread safety
#include <atomic>
//vectors and arrays are required for lists
#include <vector>
#include <array>
//add a mutex to make threads safe
#include <mutex>
#include <shared_mutex>
//use a weighted queue to queue the jobs
#include <queue>

//threads are required for multi-threaded work loads
#include <thread>
//conditional variables are used for work queueing
#include <condition_variable>

/**
 * @brief store different load priorities
 */
enum AssetLoadPriority : int8_t {
    //"normal" load priority
    GLGE_ASSET_LOAD_PRIORITY_NORMAL = 0,
    //store the "high" load priority
    GLGE_ASSET_LOAD_PRIORITY_HIGH = 1,
    //store the "low" load priority
    GLGE_ASSET_LOAD_PRIORITY_LOW = -1
};

/**
 * @brief define a structure to queue an async load job
 */
struct AssetAsyncLoadJob
{
    //store a handle to the object
    //this also makes sure that the object is not deleted unexpectedly
    AssetHandle handle;
    //store the priority of the asset
    AssetLoadPriority priority;
    //store a function callback to signal when the load is done
    void (*callback)(AssetHandle, bool);
};

/**
 * @brief store what a job entry looks like
 */
struct AssetJobRegistryEntry 
{
    /**
     * @brief store the state of the job
     */
    enum class State : uint8_t {
        //the job is currently not started
        UNINITIALIZED = 0,
        //the job is currently running
        RUNNING = 1,
        //the job has finished
        FINISHED = 2
    };

    //store the version of the job entry
    std::atomic<uint32_t> version{1};
    //store if the job was canceled
    std::atomic_bool canceled{false};
    //store the state of the job
    std::atomic<State> state{State::UNINITIALIZED};
    //store the job loading
    AssetAsyncLoadJob job;

    //create a simple constructor for an entry
    AssetJobRegistryEntry(const AssetAsyncLoadJob& _job)
     : job(_job)
    {}

    //create a copy and a move operator
    AssetJobRegistryEntry(const AssetJobRegistryEntry& o) noexcept
     : version(o.version.load(std::memory_order::acquire)), 
       canceled(o.canceled.load(std::memory_order::acquire)), 
       state(o.state.load(std::memory_order::acquire)),
       job(o.job)
    {}
    AssetJobRegistryEntry(AssetJobRegistryEntry&& o) noexcept
     : version(o.version.load(std::memory_order::acquire)), 
       canceled(o.canceled.load(std::memory_order::acquire)), 
       state(o.state.load(std::memory_order::acquire)),
       job(o.job)
    {}

    //a utility function to cancel the task
    inline void cancel() noexcept {canceled.store(true, std::memory_order::relaxed);}
    //a utility function to check if the task was canceled
    inline bool isCanceled() const noexcept {return canceled.load(std::memory_order::acquire);}

};

/**
 * @brief store the handle of a job
 */
struct AssetJobHandle {
    //store the index of thee job
    uint32_t index;
    //store the version of the job (done so the index can be re-used and the duplication can be detected)
    uint32_t version;
    //store the priority
    AssetLoadPriority priority;

    /**
     * @brief compare the handle priorities
     * 
     * @param handle a constant reference to the handle to compare with
     * @return true : the handle is of higher priority
     * @return false : the handle is of lower or equal priority
     */
    inline bool operator<(const AssetJobHandle& handle) const noexcept 
    {return handle.priority < priority;}
};


/**
 * @brief store a class to fully handle the asset streaming
 * 
 * The class is fully private and can only be used by itself and the friended classes. 
 */
class AssetJobRegistry {
private:

    //the asset manage can interface with the job registry so add it as a friend
    friend class AssetManager;

    /**
     * @brief enqueue a new job to load an asset async later
     * 
     * @param job the job to enqueue
     * @return AssetJobHandle the handle to this new job
     */
    static AssetJobHandle enqueue(const AssetAsyncLoadJob& job) noexcept;

    /**
     * @brief remove a job from the job pool
     * 
     * @param job the handle of the job to FULLY delete
     */
    static void popJob(const AssetJobHandle& job) noexcept;

    /**
     * @brief check if a job handle is valid
     * 
     * @param handle the handle to check
     * @return true : the handle is valid
     * @return false : the handle is not valid
     */
    static bool isValid(const AssetJobHandle& handle) noexcept;

    /**
     * @brief a safe, but NOT thread-safe function to check if a handle is valid
     * 
     * @param handle the handle to check for validity
     * @return true : the handle is valid
     * @return false : the handle is not valid
     */
    inline static bool isValid_Unsafe(const AssetJobHandle& handle) noexcept 
    {
        //check if the handle points to the current asset version
        return (handle.index < m_jobs.size()) ? (m_jobs[handle.index].version.load(std::memory_order::acquire) == handle.version) : false;
    }

    /**
     * @brief cancel a job
     * 
     * @param handle the handle of the job to cancel
     * @return true : the job was canceled successfully
     * @return false : failed to cancel the job
     */
    static bool cancelJob(const AssetJobHandle& handle) noexcept;

    //store all the jobs
    inline static std::vector<AssetJobRegistryEntry> m_jobs;
    //store a list of all free jobs
    inline static std::vector<uint32_t> m_freeList;
    //use a mutex to make sure the vector is not moved while reading
    inline static std::shared_mutex m_jobMutex;

    //store a queue of all the jobs
    inline static std::priority_queue<AssetJobHandle> m_jobQueue;
    //use a mutex to make the job queue thread safe
    inline static std::shared_mutex m_queueMutex;

    //store the thread pool
    inline static std::vector<std::thread> m_threadPool;
    //store if the asset job registry is running
    inline static std::atomic_bool m_running{true};
    //store if a job is available
    inline static std::condition_variable_any m_jobAvailable;

    /**
     * @brief this is the function that runs on all worker threads
     */
    static void worker_ThreadFunction() noexcept;

    //private constructor / destructor

    /**
     * @brief initialize the asset job registry
     */

    static void initialize() noexcept;

    /**
     * @brief clean up the asset job registry
     */
    static void cleanup() noexcept;

    //store a single global instance to work similar to a singleton
    static AssetJobRegistry REGISTRY;

};

#endif

#endif