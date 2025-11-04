/**
 * @file AssetManager.h
 * @author DM8AT
 * @brief define the structure for a global asset manager that is responsible for all assets
 * @version 0.1
 * @date 2025-10-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_ASSETS_ASSET_MANAGER_
#define _GLGE_CORE_ASSETS_ASSET_MANAGER_

//add assets
#include "Asset.h"

//Define the class for C++
#if __cplusplus

//add vectors as easy dynamic arrays
#include <vector>
//use unordered maps from external -> internal mapping
#include <unordered_map>

//add atomics and a mutex to make registry entries thread safe
#include <mutex>
#include <shared_mutex>
#include <atomic>
//needed for copying
#include <cstring>
//store a list of all free spots
//they are only added if they were freed
#include <queue>

//add the asset streaming stuff
#include "AssetStream.h"

/**
 * @brief store an actual asset in the asset manager / asset registry
 */
struct AssetRegistryEntry {
    //store the asset type to avoid type confusion
    AssetTypeID type = AssetTypeRegistry::INVALID_ID;
    //store a pointer to the asset
    //if the pointer is 0, the asset is currently not used
    BaseAsset* ptr = nullptr;
    //store the amount of references
    //if this is 0, the asset MAY BE IN USE for deletion
    //if the ref count is UINT32_MAX that means that there are no used elements after this one
    std::atomic_uint32_t refCount{0};
    //synchronization of the asset must be done by the asset
    //since the mutex can't be copied

    /**
     * @brief Construct a new Asset Registry Entry
     */
    AssetRegistryEntry() noexcept = default;

    /**
     * @brief Construct a new Asset Registry Entry
     * 
     * Copy constructor for the entry
     * 
     * @param entry the entry to copy from
     */
    AssetRegistryEntry(const AssetRegistryEntry& entry) noexcept 
       //just copy all the data over
     : type(entry.type),
       ptr(entry.ptr),
       refCount(entry.refCount.load(std::memory_order_relaxed))
    {}

    /**
     * @brief copy operator overload
     * 
     * @param entry the element to copy
     * @return AssetRegistryEntry& a reference to the new element
     */
    AssetRegistryEntry& operator=(const AssetRegistryEntry& entry) noexcept {
        //just copy all the data over
        type = entry.type;
        ptr = entry.ptr;
        refCount.store(entry.refCount.load(std::memory_order_relaxed), std::memory_order::relaxed);
        //return self
        return *this;
    }

    /**
     * @brief Construct a new Asset Registry Entry
     * 
     * @param _type the type index of the asset
     * @param _ptr a pointer to the actual asset implementation
     * @param _destructor the destructor to call for the asset
     */
    AssetRegistryEntry(const AssetTypeID& _type, BaseAsset* _ptr)
     : type(_type), ptr(_ptr), refCount(0)
    {}
};

/**
 * @brief a static class to handle the global asset storage
 * 
 * Not a namespace because a namespace does not allow protected
 */
class AssetManager final
{
public:

    /**
     * @brief a function to create a new asset
     * 
     * @tparam T the type of asset to create
     * @tparam Args the arguments to pass to the asset creation function
     * @param args the arguments to pass to the asset creation function
     * @return AssetHandle a handle to the new asset
     */
    template <typename T, typename ...Args>
    static inline AssetHandle create(Args ...args) {
        //the asset must be a valid asset
        static_assert(std::is_base_of_v<BaseAsset, T>, "The class must derive public from the \"BaseAsset\" class to be used as an asset");
        //add a new entry and return the new handle
        return addNewEntry(AssetRegistryEntry(
            AssetTypeRegistry::getID<T>(), 
            new T(std::forward<Args>(args) ...)
        ));
    }

    /**
     * @brief check if a handle is valid
     * 
     * @param handle the handle to check
     * @return true : the handle is valid
     * @return false : the handle is not valid
     */
    inline static bool isValid(const AssetHandle& handle) noexcept 
    {return handle.index < m_versions.size() && m_versions[handle.index] == handle.version;}

    /**
     * @brief Get access to a specific asset
     * 
     * @warning this function is private so only asset handles may access it
     * This is done so the handle MUST be valid to 
     * 
     * @tparam T the type of asset to quarry
     * @param handle a constant reference to the handle to quarry for
     * @return T* a pointer to the actual asset
     */
    template <typename T>
    static inline T* getAsset(const AssetHandle& handle) noexcept {
        std::shared_lock lock(m_assetMutex);
        //sanity check the handle first
        if (!isValid(handle)) {return nullptr;}
        //directly return the pointer of the element and cast it correctly
        return (T*)m_assets[handle.index].ptr;
    }

    /**
     * @brief wait for a specific asset to be fully loaded
     * 
     * @param handle the asset handle of the asset to load
     */
    static void waitForLoad(const AssetHandle& handle) noexcept;

    /**
     * @brief call all queued up destructors
     */
    static void flushDestructors() noexcept;

    /**
     * @brief clean up everything related to the asset system
     * 
     * @warning this deletes all assets, regardless if a handle exists
     */
    static void shutdown() noexcept;

private:

    //give access to the asset job registry
    friend class AssetJobRegistry;

    /**
     * @brief a static initialization function that should only run once
     * 
     * This function should run when the first entity is created
     */
    static void init() noexcept;

    /**
     * @brief add a new entry to the asset manager
     * 
     * @param entry a constant reference to the entry to add. This will be COPIED to the asset storage
     * @param path the data that is used to create the actual asset
     * @return AssetHandle the handle of the new asset
     */
    static AssetHandle addNewEntry(const AssetRegistryEntry& entry) noexcept;

    /**
     * @brief a function to register a new handle
     * 
     * @param handle the handle to register
     */
    static void registerHandle(const AssetHandle& handle) noexcept;

    /**
     * @brief un-register a handle
     * 
     * @param handle the handle to un-register
     */
    static void unregisterHandle(const AssetHandle& handle) noexcept;


    //use friend to give the handle access to the handle interface
    friend struct AssetHandle;

    /**
     * @brief store the actual assets
     * 
     * Make sure the list is continues in memory (some un-used elements may be in between)
     * DO not give out references or pointers to the data IT MAY MOVE
     */
    inline static std::vector<AssetRegistryEntry> m_assets{};
    /**
     * @brief store a mutex to make the asset vector does not resize all of a sudden
     */
    inline static std::shared_mutex m_assetMutex;

    /**
     * @brief store a list of all free indices in the list
     * 
     * The free list is used as a filo (first-in-last-out) queue to avoid memmove operations when the element is erased
     */
    inline static std::vector<uint32_t> m_freeList;
    /**
     * @brief store a list of all the versions
     */
    inline static std::vector<uint32_t> m_versions;

    /**
     * @brief store a list of all elements that still need to be deleted
     */
    inline static std::vector<BaseAsset*> m_pendingDestroy;
    /**
     * @brief a mutex used to make sure that cleaning up is safe
     */
    inline static std::shared_mutex m_destroyMutex;


    //use a private static class instance to ensure that the shutdown and init methods are called
    
    /**
     * @brief Construct a new Asset Manager
     */
    AssetManager() noexcept;

    /**
     * @brief Destroy the Asset Manager
     */
    ~AssetManager() noexcept;

    /**
     * @brief store the asset manager to ensure safe clean up / creation
     */
    static AssetManager m_manager;
    /**
     * @brief store if the asset manager is initialized
     */
    inline static bool m_initialized = false;

};

#endif

#endif