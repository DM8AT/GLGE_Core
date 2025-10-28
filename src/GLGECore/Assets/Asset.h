/**
 * @file Asset.h
 * @author DM8AT
 * @brief define what an asset is
 * @version 0.1
 * @date 2025-10-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_ASSETS_ASSET_
#define _GLGE_CORE_ASSETS_ASSET_

//add asset types
#include "AssetType.h"

//add the string C binding
#include "../../GLGE_BG/CBinding/String.h"
//add debugging
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

/**
 * @brief a type to store an asset identifier
 */
typedef uint64_t AssetID;

//define an invalid value for the asset id
#define GLGE_ASSET_ID_INVALID UINT64_MAX

/**
 * @brief store the state of an asset
 */
typedef enum e_AssetState {
    //the asset is currently not loaded
    ASSET_STATE_UNLOADED = 0,
    //the asset is currently loading
    ASSET_STATE_LOADING,
    //the asset is fully loaded
    ASSET_STATE_LOADED,
    //the asset failed to load
    ASSET_STATE_FAILED
} AssetState;

//only add handles for C++ to ensure that they are as opaque as possible
#if __cplusplus

//add atomics to store atomics in assets
#include <atomic>

//define an asset handle 
//it wraps an asset ID and has special operator overloads for reference counting
struct AssetHandle {
    /**
     * @brief Construct a new Asset Handle
     */
    AssetHandle() = default;

    /**
     * @brief Construct a new Asset Handle
     * 
     * @param handle a constant reference to the asset to copy
     */
    AssetHandle(const AssetHandle& handle) noexcept;

    /**
     * @brief Construct a new Asset Handle
     * 
     * @param handle a move reference to the handle to move
     */
    AssetHandle(AssetHandle&& handle) noexcept;

    /**
     * @brief Destroy the Asset Handle
     * 
     * @warning this MAY invalidate the asset the handle is pointing to (reference counted)
     */
    ~AssetHandle() noexcept;

    /**
     * @brief asign this asset handle another asset handle
     * 
     * Copy the data over
     * 
     * @param handle the handle to copy
     * @return s_AssetHandle& a reference to this handle
     */
    AssetHandle& operator=(const AssetHandle& handle) noexcept;

    /**
     * @brief asign this asset handle another asset handle
     * 
     * @param handle the handle to move over
     * @return s_AssetHandle& a reference to this handle
     */
    AssetHandle& operator=(AssetHandle&& handle) noexcept;

private:

    //add the asset manager as a friend class
    friend class AssetManager;
    //also add the asset job registry as a friend class
    friend class AssetJobRegistry;

    /**
     * @brief Construct a new Asset Handle
     * 
     * @param index the actual index to the map of the asset
     * @param version the version of the handle (used to identify if the handle points to an old object)
     */
    AssetHandle(uint32_t _index, uint32_t _version) noexcept
     : index(_index), version(_version)
    {}

    //store a marker for the invalid index
    static constexpr uint32_t INDEX_INVALID = UINT32_MAX;

    //store the version of the handle (to ensure that the handle actually wants the correct object)
    uint32_t version = 0;
    //store the index into the asset map
    uint32_t index = INDEX_INVALID;

};

class BaseAsset {
public:

    /**
     * @brief Construct a new Base Asset
     */
    BaseAsset() = default;

    /**
     * @brief Destroy the Base Asset
     */
    virtual ~BaseAsset() = default;

    /**
     * @brief load the asset
     */
    virtual void load() noexcept = 0;

    /**
     * @brief Get the Load State of the asset
     * 
     * @return const AssetState& the loading state of the asset
     */
    inline AssetState getLoadState() const noexcept {return m_state.load(std::memory_order::acquire);}

protected:

    /**
     * @brief update the current loading state
     * 
     * @param next the state to make the next state
     */
    inline const void updateLoadState(const AssetState& next) {m_state.store(next, std::memory_order::release);}

    //store the current state of the asset
    std::atomic<AssetState> m_state{AssetState::ASSET_STATE_UNLOADED};

};

#endif

#endif