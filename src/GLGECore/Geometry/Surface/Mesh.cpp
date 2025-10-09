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

//include AABB's and Spheres to create bounding volume creation functions
#include "../Volumes/AABB.h"
#include "../Volumes/Sphere.h"

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

Mesh::~Mesh()
{
    //if the vertices exist, delete the array
    if (m_vertices) {
        delete[] (uint8_t*)m_vertices;
        m_vertices = NULL;
    }
}

template <> AABB Mesh::getBoundingVolume<AABB>() const noexcept {
    //store the AABB to return
    AABB ret;
    //calculate the offset of the position element
    uint64_t idx = m_layout.getIndexOfElement(VERTEX_ELEMENT_TYPE_POSITION);
    //if no position exists, what?
    if (idx == UINT64_MAX) {return AABB{};}
    //get the offset in bytes of the element
    uint64_t offs = m_layout.getOffsetOf(idx);
    //get the type of the element
    VertexElementDataType dat = m_layout.m_elements[idx].data;
    //iterate over all vertices (indices are not important, only positions matter)
    for (size_t i = 0; i < m_vertexCount; ++i) {
        //get the element position
        uint8_t* pos_ptr = &(((uint8_t*)m_vertices)[i*m_layout.m_size + offs]);

        //store the position as a 3D vector
        vec3 pos;
        switch (dat)
        {
            case VERTEX_ELEMENT_DATA_TYPE_INT8:
            {
                int32_t v = *((int8_t*)pos_ptr);
                pos = vectorCast<vec3>(v);
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_UINT8:
            {
                uint32_t v = *((uint8_t*)pos_ptr);
                pos = vectorCast<vec3>(v);
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_INT16:
            {
                int32_t v = *((int16_t*)pos_ptr);
                pos = vectorCast<vec3>(v);
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_UINT16:
            {
                uint32_t v = *((uint16_t*)pos_ptr);
                pos = vectorCast<vec3>(v);
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_INT32:
            {
                int32_t v = *((int32_t*)pos_ptr);
                pos = vectorCast<vec3>(v);
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_UINT32:
            {
                uint32_t v = *((uint32_t*)pos_ptr);
                pos = vectorCast<vec3>(v);
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_FLOAT:
            {
                float v = *((float*)pos_ptr);
                pos = vectorCast<vec3>(v);
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_DOUBLE:
            {
                double v = *((double*)pos_ptr);
                pos = vectorCast<vec3>(v);
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC2:
            {
                float* v = (float*)pos_ptr;
                pos = vectorCast<vec3>(vec2(v[0], v[1]));
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC3:
            {
                pos = *((vec3*)pos_ptr);
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC4:
            {
                float* v = (float*)pos_ptr;
                pos = vectorCast<vec3>(vec4(v[0], v[1], v[2], v[3]));
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC2:
            {
                double* v = (double*)pos_ptr;
                pos = vectorCast<vec3>(vec2((float)v[0], (float)v[1]));
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC3:
            {
                double* v = (double*)pos_ptr;
                pos = vectorCast<vec3>(vec3((float)v[0], (float)v[1], (float)v[2]));
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC4:
            {
                double* v = (double*)pos_ptr;
                pos = vectorCast<vec3>(vec4((float)v[0], (float)v[1], (float)v[2], (float)v[3]));
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_INT32_VEC2:
            {
                int32_t* v = (int32_t*)pos_ptr;
                pos = vectorCast<vec3>(vec2(v[0], v[1]));
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_INT32_VEC3:
            {
                int32_t* v = (int32_t*)pos_ptr;
                pos = vectorCast<vec3>(vec3(v[0], v[1], v[2]));
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_INT32_VEC4:
            {
                int32_t* v = (int32_t*)pos_ptr;
                pos = vectorCast<vec3>(vec4(v[0], v[1], v[2], v[3]));
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC2:
            {
                uint32_t* v = (uint32_t*)pos_ptr;
                pos = vectorCast<vec3>(vec2(v[0], v[1]));
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC3:
            {
                uint32_t* v = (uint32_t*)pos_ptr;
                pos = vectorCast<vec3>(vec3(v[0], v[1], v[2]));
                break;
            }
            case VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC4:
            {
                uint32_t* v = (uint32_t*)pos_ptr;
                pos = vectorCast<vec3>(vec4(v[0], v[1], v[2], v[3]));
                break;
            }
        
        default:
            //return the default AABB for an unsupported type
            return AABB{};
        }

        //merge the point to the AABB
        ret.merge(pos);
    }

    //return the finished AABB
    return ret;
}

template <> Sphere Mesh::getBoundingVolume<Sphere>() const noexcept {
    //first, get the AABB
    AABB box = getBoundingVolume<AABB>();
    //then, create and return a sphere that encloses the whole box
    return Sphere((box.min + box.max) * 0.5f, length((box.max - box.min) * 0.5f));
}



Mesh* mesh_Create(void* vertices, uint64_t vertexCount, const VertexLayout* layout, index_t* indices, uint64_t indexCount)
{return new Mesh(vertices, vertexCount, *layout, indices, indexCount);}

void mesh_Delete(Mesh* mesh) {delete mesh;}

const VertexLayout* mesh_GetVertexLayout(Mesh* mesh) {return &mesh->getVertexLayout();}

void* mesh_GetVertices(Mesh* mesh) {return mesh->getVertices();}

uint64_t mesh_GetVertexCount(Mesh* mesh) {return mesh->getVertexCount();}

index_t* mesh_GetIndices(Mesh* mesh) {return mesh->getIndices();}

uint64_t mesh_GetIndexCount(Mesh* mesh) {return mesh->getIndexCount();}
