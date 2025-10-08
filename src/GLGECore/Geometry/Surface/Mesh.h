/**
 * @file Mesh.h
 * @author DM8AT
 * @brief define what a mesh is. A mesh defines the surface of an object. 
 * @version 0.1
 * @date 2025-10-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_GEOMETRY_SURFACE_MESH_
#define _GLGE_CORE_GEOMETRY_SURFACE_MESH_

//include the type definitions
#include "../../Types.h"

//define the type for indices
typedef uint32_t index_t;

//include vertex layouts
#include "VertexLayout.h"

#if __cplusplus

//include resizable containers
#include <vector>

/**
 * @brief store a simple mesh
 */
class Mesh
{
public:

    /**
     * @brief Construct a new Mesh
     * 
     * @param vertices a C array containing some form of vertex structure
     * @param vertexCount the amount of vertices contained in the array
     * @param layout the layout of a vertex element
     * @param indices a pointer to an array of index types
     * @param indexCount the amount of indices in the index array
     */
    Mesh(void* vertices, uint64_t vertexCount, const VertexLayout& layout, index_t* indices, uint64_t indexCount);

    /**
     * @brief Construct a new Mesh
     * 
     * @param vertices a C array containing some form of vertex structure
     * @param vertexCount the amount of vertices contained in the array
     * @param layout the layout of a vertex element
     * @param indices a std::vector of index_t types for the indices
     */
    inline Mesh(void* vertices, uint64_t vertexCount, const VertexLayout& layout, std::vector<index_t> indices)
     : Mesh(vertices, vertexCount, layout, indices.data(), indices.size())
    {}

    /**
     * @brief Destroy the Mesh
     */
    ~Mesh();

    /**
     * @brief Get the Vertex Layout of the mesh
     * 
     * @return constexpr const VertexLayout& a constant reference to the vertex layout of the mesh
     */
    inline constexpr const VertexLayout& getVertexLayout() const noexcept {return m_layout;}

    /**
     * @brief Get the Vertices of the mesh
     * 
     * @return void* a pointer to the raw vertex data
     */
    inline constexpr void* getVertices() const noexcept {return m_vertices;}

    /**
     * @brief Get the Vertices of the mesh
     * 
     * @tparam T the type for the vertex data
     * @return constexpr T* a pointer to the raw vertex data
     */
    template <typename T> inline constexpr T* getVertices() const noexcept {return (T*)m_vertices;}

    /**
     * @brief Get the amount of vertices
     * 
     * @return constexpr uint64_t the amount of vertices in the model
     */
    inline constexpr uint64_t getVertexCount() const noexcept {return m_vertexCount;}

    /**
     * @brief Get the Indices of the model
     * 
     * @return constexpr index_t* a pointer to the raw index data
     */
    inline constexpr index_t* getIndices() const noexcept {return (index_t*)m_indices.data();}

    /**
     * @brief Get the amount of indices in the model
     * 
     * @return constexpr uint64_t the amount of indices in the model
     */
    inline constexpr uint64_t getIndexCount() const noexcept {return m_indices.size();}

protected:

    //store the layout of the vertices
    VertexLayout m_layout;
    //store the amount of stored vertices
    uint64_t m_vertexCount = 0;
    //store a pointer to the vertex array
    void* m_vertices = NULL;
    //store the indices of the mesh
    std::vector<index_t> m_indices;

};

#else //else, define an opaque data structure

//define an opaque (MSVC requires the unused byte) for a mesh
typedef struct s_Mesh {byte unused;} Mesh;

#endif

/**
 * @brief Construct a new Mesh
 * 
 * @param vertices a C array containing some form of vertex structure
 * @param vertexCount the amount of vertices contained in the array
 * @param layout the layout of a vertex element
 * @param indices a pointer to an array of index types
 * @param indexCount the amount of indices in the index array
 * 
 * @return a pointer to a new mesh
 */
Mesh* mesh_Create(void* vertices, uint64_t vertexCount, const VertexLayout* layout, index_t* indices, uint64_t indexCount);

/**
 * @brief delete an existing mesh instance
 * 
 * @param mesh a pointer to the mesh instance to delete
 */
void mesh_Delete(Mesh* mesh);

/**
 * @brief get the vertex layout of a mesh
 * 
 * @param mesh a pointer to the mesh to quarry the data from
 * @return const VertexLayout* a pointer to the vertex layout of the mesh
 */
const VertexLayout* mesh_GetVertexLayout(Mesh* mesh);

/**
 * @brief get the vertices of a mesh
 * 
 * @param mesh a pointer to the mesh to quarry the data from
 * @return void* a pointer to the vertices of the mesh
 */
void* mesh_GetVertices(Mesh* mesh);

/**
 * @brief get the amount of vertices of a mesh
 * 
 * @param mesh a pointer to the mesh to quarry the data from
 * @return uint64_t the amount of vertices of the mesh
 */
uint64_t mesh_GetVertexCount(Mesh* mesh);

/**
 * @brief get the indices of a mesh
 * 
 * @param mesh a pointer to the mesh to quarry the data from
 * @return index_t* a pointer to the indices of the mesh
 */
index_t* mesh_GetIndices(Mesh* mesh);

/**
 * @brief get the amount of indices of a mesh
 * 
 * @param mesh a pointer to the mesh to quarry the data from
 * @return uint64_t get the amount of indices of a mesh
 */
uint64_t mesh_GetIndexCount(Mesh* mesh);

#endif