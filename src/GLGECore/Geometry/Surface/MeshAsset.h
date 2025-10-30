/**
 * @file MeshAsset.h
 * @author DM8AT
 * @brief define what the mesh overload for an asset looks like
 * @version 0.1
 * @date 2025-10-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_GEOMETRY_SURFACE_MESH_ASSET_
#define _GLGE_CORE_GEOMETRY_SURFACE_MESH_ASSET_

//add assets
#include "../../Assets/Assets.h"
//add the mesh system
#include "Mesh.h"
//add strings
#include "../../../GLGE_BG/CBinding/String.h"

//only available for C++
#if __cplusplus

/**
 * @brief define what a mesh asset is
 */
class MeshAsset final : public BaseAsset
{
public:

    /**
     * @brief Construct a new Mesh Asset
     * 
     * @warning this will only load GLGE mesh assets. This will not import other file formats. 
     * 
     * @param path the path to the asset file to load
     */
    MeshAsset(const String& path);

    /**
     * @brief import an external mesh file and convert it to a mesh asset
     * 
     * @warning this checks if an asset file with that name does exist. If it does, it just returns the path directly
     * 
     * @param path the path to the file to import
     * @param suffix the suffix to give to the imported file
     * @return String the path to the imported file or an empty string if the import failed
     */
    static String import(const String& path, const String& suffix = "gm") noexcept;

    /**
     * @brief access the underlying mesh
     * 
     * @return Mesh& a reference to the stored mesh
     */
    inline Mesh& mesh() noexcept {return *m_ptr;}

    /**
     * @brief access the mesh stored in the asset
     * 
     * @return Mesh& a reference to the stored mesh
     */
    inline Mesh& operator()(void) noexcept {return *m_ptr;}

    /**
     * @brief get access to the mesh as a pointer
     * 
     * @return Mesh* a pointer to the stored mesh
     */
    inline Mesh* operator->(void) noexcept {return m_ptr;}

    /**
     * @brief load the actual mesh asset from disk
     */
    virtual void load() noexcept override;

private:

    /**
     * @brief store the actual mesh
     */
    uint8_t m_mesh[sizeof(Mesh)];
    /**
     * @brief store a pointer to the mesh (stored in m_mesh) or null if the mesh is not set up
     */
    Mesh* m_ptr = nullptr;
    /**
     * @brief store a path to a mesh asset
     */
    String m_path;

};

#endif

#endif