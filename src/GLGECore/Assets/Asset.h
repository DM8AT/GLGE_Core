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

//include the default types
#include "../Types.h"
//add the string C binding
#include "../../GLGE_BG/CBinding/String.h"
//add debugging
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

/**
 * @brief a type to store an asset identifier
 */
typedef uint64_t AssetID;
/**
 * @brief define how the types of assets are stored
 */
typedef uint32_t AssetTypeID;

/**
 * @brief define the data the asset is holding
 * 
 * That is what defines an asset
 */
typedef struct s_AssetData {
    //store an unique id for each asset
    AssetID id;
    //store the path of the asset (if it is a file)
    String path;
    //store the type of asset
    AssetTypeID type;
} AssetData;

//add stuff that is only available for C++
#if __cplusplus

//add type info
#include <typeindex>
//get limits of types
#include <limits>
//add arrays
#include <array>

//add constexpr capable hash maps
#include ""

/**
 * @brief a class to store and handle the mapping from actual types to type ids
 */
class AssetTypeRegistry final {
public:

    //store the maximum number of asset types that are allowed
    static constexpr size_t MAX_TYPES = 512;
    //store an ID to notify that it is invalid
    static constexpr AssetTypeID INVALID_ID = std::numeric_limits<AssetTypeID>::max();

    /**
     * @brief get the type ID of a specific type
     * 
     * @tparam T the type to evaluate
     * @return consteval AssetTypeID the ID of the asset's type
     */
    template <typename T>
    static consteval AssetTypeID getID() noexcept {
        //get the type hash of the type
        constexpr uint64_t hash = typeHash<T>();
        //maybe it allready exists
        constexpr auto ptr = table.find(hash);
        //check if it allready exists. If it does, return it
        if (ptr) {return *ptr;}
        //else, make sure that there is enough space and then add it
        static_assert(next_ID < MAX_TYPES, "AssetTypeRegistry overflow: too many registered asset types");
        return registerType<T>();
    }

    /**
     * @brief Get the Hash of a specific type
     * 
     * @tparam T the type to get the hash for
     * @return consteval uint64_t the hash of the type
     */
    template <typename T>
    static consteval uint64_t getHash() noexcept
    {return typeHash<T>();}

private:

    /**
     * @brief compute the hash for a specific type
     * 
     * This function uses the templated function name to use the type name indirectly to compute unique hashes for all types
     * 
     * @tparam T the type to compute the hash for
     * @return consteval uint64_t the hash of the type
     */
    template <typename T>
    static consteval uint64_t typeHash() noexcept {
    #if defined(_MSC_VER)
        constexpr std::string_view name = __FUNCSIG__;
    #elif defined(__clang__) || defined(__GNUC__)
        constexpr std::string_view name = __PRETTY_FUNCTION__;
    #else
        //error - unsupported compiler
        #error "Unsupported compiler. Please inform the maintainer of GLGE to add the required compiler"
    #endif
        // FNV-1a 64-bit hash
        uint64_t hash = 1469598103934665603ull;
        for (unsigned char c : name)
            hash = (hash ^ c) * 1099511628211ull;
        return hash;
    }

    //sanity check the hash function
    static_assert(typeHash<int>() != typeHash<float>(), "Invalid hashing. Please check your compiler settings.");

    /**
     * @brief register a type
     * 
     * @tparam T the type to register
     * @return consteval AssetTypeID the type id registered to this type
     */
    template <typename T>
    static consteval AssetTypeID registerType() noexcept {
        //compute the hash
        constexpr uint64_t hash = typeHash<T>();
        AssetTypeID id = next_ID++;
        //insert the entry to the table
        table.insert(hash, id);
        return id;
    }

    //store the hash table of type hashs to type IDs
    inline static constexpr Constexpr_HashMap<uint64_t, AssetTypeID, MAX_TYPES> table{};
    //store the next id that will be used
    inline static constexpr AssetTypeID next_ID = 0;

};

#endif

#endif