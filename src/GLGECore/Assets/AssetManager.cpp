/**
 * @file AssetManager.cpp
 * @author DM8AT
 * @brief 
 * @version 0.1
 * @date 2025-10-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//add the asset manager
#include "AssetManager.h"
//add the asset stream
#include "AssetStream.h"
//add debug stuff
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

//use a static instance to ensure correct setup / shutdown
AssetManager AssetManager::m_manager;

//initialize the asset manager
AssetManager::AssetManager() noexcept {
    //prevent multiple inits
    if (m_initialized){return;}
    //run the initializer
    init();
    AssetJobRegistry::initialize();

    //say that it's now initialized
    m_initialized = true;
}

//clean up the asset manger
AssetManager::~AssetManager() noexcept {
    //prevent multiple shutdown
    if (!m_initialized) {return;}
    //actual shutdown call
    shutdown();
    AssetJobRegistry::cleanup();
    //say that it is now shut down
    m_initialized = false;
}

void AssetManager::init() noexcept
{
    //reserve enough space in the vector (4096 elements)
    //only RESERVE, not resize
    m_assets.reserve(4096);
    m_versions.reserve(4096);
}

void AssetManager::waitForLoad(const AssetHandle& handle) noexcept
{
    //if the handle is not valid, just stop
    if (!isValid(handle)) {return;}

    //simple, but effective - just wait
    while(true) {
        //quarry the state
        AssetState state = ASSET_STATE_UNLOADED;
        {
            //lock so nothing is moved unexpectedly
            std::shared_lock lock(m_assetMutex);
            //sanity-check again
            if (!isValid(handle)) {return;}
            //now, check if the element is marked as "loaded" or "failed"
            state = m_assets[handle.index].ptr->getLoadState();
        }

        //check the state
        if ((state == ASSET_STATE_LOADED) || (state == ASSET_STATE_FAILED)) {return;}

        //wait for a bit to not fully use a single core
        //but only a small time step (about 100 µs)
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

AssetHandle AssetManager::addNewEntry(const AssetRegistryEntry& entry) noexcept
{
    //store the raw ID and the version
    uint32_t newID = 0;
    uint32_t version = 0;
    //inside this scope the handle map is locked
    {
        //in this section there may be resizes to the vector (that depend on a condition)
        //so the asset mutex is locked to make sure that only a single thread does that
        std::unique_lock assetLock(m_assetMutex);

        //check if adding an element is required
        if (m_freeList.empty()) {
            //use the new index directly
            newID = m_assets.size();
            //add a new element
            m_assets.emplace_back();
            m_versions.emplace_back(1);
        }
        else {
            //get thee next free index
            newID = m_freeList.back();
            m_freeList.pop_back();
        }
        
        //insert the new entry
        m_assets[newID].type = entry.type;
        m_assets[newID].ptr = entry.ptr;
        //store 1 entry. This is for the first handle ever created
        m_assets[newID].refCount.store(1, std::memory_order::relaxed);
        //store the version of the asset to create the handle
        version = m_versions[newID];
    }
    //fence the data update
    std::atomic_thread_fence(std::memory_order::release);
    //store the new handle
    AssetHandle handle(newID, version);
    //queue the loading job
    AssetJobRegistry::enqueue(AssetAsyncLoadJob{
        .handle = handle,
        .priority = GLGE_ASSET_LOAD_PRIORITY_NORMAL,
        .callback = nullptr
    });
    //return the handle of the new asset
    return handle;
};

void AssetManager::registerHandle(const AssetHandle& handle) noexcept
{
    //sanity check the handle
    if (!isValid(handle)) {return;}
    //valid handle. Increase the version.
    m_assets[handle.index].refCount.fetch_add(1, std::memory_order::relaxed);
}

void AssetManager::unregisterHandle(const AssetHandle& handle) noexcept
{
    //sanity check the handle
    if (!isValid(handle)) {return;}

    //unregister the handle and check if there are still references left
    uint32_t idx = handle.index;
    uint32_t prev = m_assets[idx].refCount.fetch_sub(1, std::memory_order::acq_rel);
    if (prev > 1) {return;}

    //lock the asset array for resizing
    {
        std::unique_lock destrLock(m_destroyMutex);
        std::unique_lock assetLock(m_assetMutex);

        //increase the generation of the asset
        ++m_versions[idx];

        //add the destructor to the pending destruction list
        m_pendingDestroy.emplace_back(m_assets[idx].ptr);

        //clear entry and add it to the free list
        m_assets[idx] = AssetRegistryEntry();
        m_freeList.push_back(idx);
    }
}

void AssetManager::flushDestructors() noexcept
{
    //store a local list of destructors to call
    std::vector<BaseAsset*> toDestroy;
    
    {
        //thread safety
        std::unique_lock lock(m_destroyMutex);
        //move the data to the local vector
        toDestroy.swap(m_pendingDestroy);
    }

    //iterate over all destructors and just execute them
    for (const auto& des : toDestroy) {
        //make sure the destructor is sane. If it is, call it. 
        if (des) {delete des;}
    }
}

void AssetManager::shutdown() noexcept
{
    //make sure no other thread may potentially do something
    std::unique_lock lock(m_assetMutex);
    //clean up all queued deletions
    flushDestructors();
    //clean up all remaining objects
    for (const auto& asset : m_assets) {
        if (asset.ptr) {delete asset.ptr;}
    }
    //clean up
    m_assets.clear();
    m_versions.clear();
    m_freeList.clear();
    m_pendingDestroy.clear();
}