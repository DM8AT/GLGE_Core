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
    //store the bitangent of the vertex
    vec3 bitangent;
    //store the texture coordinate for the vertex
    vec2 tex;
} Vertex;

#endif