/**
 * @file Asset.cpp
 * @author DM8AT
 * @brief implement the operators for the asset handle
 * @version 0.1
 * @date 2025-10-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the base asset stuff
#include "Asset.h"
//also add the asset manager
#include "AssetManager.h"

AssetHandle::AssetHandle(const AssetHandle& handle) noexcept
   //copy the index and version over
 : index(handle.index), version(handle.version)
{
    //register the new handle
    AssetManager::registerHandle(*this);
}

AssetHandle::AssetHandle(AssetHandle&& handle) noexcept
   //copy the index and version over
 : index(handle.index), version(handle.version)
{
    //clear the other index. This removes the necessary to add the new handle (this just takes the handle)
    handle.index = INDEX_INVALID;
}

AssetHandle::~AssetHandle() noexcept
{
    //if this is NOT the invalid ID, remove the handle
    if (index != INDEX_INVALID) {
        AssetManager::unregisterHandle(*this);
        index = INDEX_INVALID;
    }
}

AssetHandle& AssetHandle::operator=(const AssetHandle& handle) noexcept
{
    //check if the old ID needs to be de-registered
    if (index != INDEX_INVALID) 
    {AssetManager::unregisterHandle(*this);}
    //copy the index and version
    index = handle.index;
    version = handle.version;
    //register the new handle
    AssetManager::registerHandle(*this);

    //return self
    return *this;
}

AssetHandle& AssetHandle::operator=(AssetHandle&& handle) noexcept
{
    //check if the old ID needs to be de-registered
    if (index != INDEX_INVALID) 
    {AssetManager::unregisterHandle(*this);}
    //copy the index and version
    index = handle.index;
    version = handle.version;
    //invalidate the other handle
    handle.index = INDEX_INVALID;
    //do NOT register the new handle. Because the other handle is invalidated
    //it won't un-register, so this just takes the handle

    //return self
    return *this;
}