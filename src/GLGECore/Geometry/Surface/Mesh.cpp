/**
 * @file Mesh.cpp
 * @author DM8AT
 * @brief implement the mesh related functions and C binding for the mesh class
 * @version 0.1
 * @date 2025-10-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the mesh api
#include "Mesh.h"

//include memory management stuff
#include <cstring>

Mesh::Mesh(void* vertices, uint64_t vertexCount, const VertexLayout& layout, index_t* indices, uint64_t indexCount)
 : m_layout(layout), m_vertexCount(vertexCount), m_vertices(new uint8_t[vertexCount * m_layout.m_size])
{
    //allocate the internal vertex buffer
    if (!m_vertices) {
        //error - allocation failed
        printf("[ERROR] Failed to allocate the the vertex buffer for a mesh. Requested size: %lu bytes\n", m_vertexCount * m_layout.m_size);
        return;
    }
    //copy the data over
    memcpy(m_vertices, vertices, m_vertexCount * m_layout.m_size);

    //resize the index buffer and copy the data over
    m_indices.resize(indexCount);
    memcpy(m_indices.data(), indices, m_indices.size() * sizeof(index_t));
}