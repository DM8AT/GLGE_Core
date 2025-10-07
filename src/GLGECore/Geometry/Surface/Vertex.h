/**
 * @file Vertex.h
 * @author DM8AT
 * @brief define what a vertex requires. Also define some defaults. 
 * @version 0.1
 * @date 2025-10-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_GEOMETRY_SURFACE_VERTEX_
#define _GLGE_CORE_GEOMETRY_SURFACE_VERTEX_

//include the vertex structures
#include "VertexLayout.h"

/**
 * @brief store the minimum vertex for most of GLGE's functionality
 */
typedef struct s_MinimumVertex {
    //the position of the vertex
    vec3 pos;
    //the normal of the vertex
    vec3 normal;
} MinimumVertex;

/**
 * @brief define the structure of a simple vertex
 */
typedef struct s_SimpleVertex {
    //store the position of the vertex
    vec3 pos;
    //store the normal vector of the vertex
    vec3 normal;
    //store a single texture coordinate
    vec2 tex;
} SimpleVertex;

/**
 * @brief a vertex ready for everything
 */
typedef struct s_Vertex {
    //store the position of the vertex
    vec3 pos;
    //store the normal vector of the vertex
    vec3 normal;
    //store the tangent of the vertex
    vec3 tangent;
    //store the texture coordinate for the vertex
    vec2 tex;
} Vertex;

//for C++ define the constant vertex layouts
#if __cplusplus

//define the layout of the minimum vertex
inline const constexpr VertexLayout GLGE_VERTEX_LAYOUT_MINIMUM_VERTEX = {
    VertexElement(VERTEX_ELEMENT_TYPE_POSITION, VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC3),
    VertexElement(VERTEX_ELEMENT_TYPE_NORMAL, VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC3),
};

//define the layout of the simple vertex
inline const constexpr VertexLayout GLGE_VERTEX_LAYOUT_SIMPLE_VERTEX = {
    VertexElement(VERTEX_ELEMENT_TYPE_POSITION, VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC3),
    VertexElement(VERTEX_ELEMENT_TYPE_NORMAL, VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC3),
    VertexElement(VERTEX_ELEMENT_TYPE_TEXTURE_COORDINATE0, VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC2),
};

//define the layout of the normal vertex
inline const constexpr VertexLayout GLGE_VERTEX_LAYOUT_VERTEX = {
    VertexElement(VERTEX_ELEMENT_TYPE_POSITION, VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC3),
    VertexElement(VERTEX_ELEMENT_TYPE_NORMAL, VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC3),
    VertexElement(VERTEX_ELEMENT_TYPE_TANGENT, VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC3),
    VertexElement(VERTEX_ELEMENT_TYPE_TEXTURE_COORDINATE0, VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC2),
};

#endif

#endif