/**
 * @file Triangle.cpp
 * @author DM8AT
 * @brief implement the functions for the triangle
 * @version 0.1
 * @date 2025-10-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the triangle
#include "Triangle.h"

//include AABB and Sphere's as bounding volumes
#include "../Volumes/AABB.h"
#include "../Volumes/Sphere.h"

template <> AABB Triangle::getBoundingVolume<AABB>() const noexcept {
    //return an AABB that contains the element wise minimum and maximum of all elements
    return AABB(
        vec3(
            (a.x < b.x) ? ((a.x < c.x) ? a.x : c.x) : ((b.x < c.x) ? b.x : c.x),
            (a.y < b.y) ? ((a.y < c.y) ? a.y : c.y) : ((b.y < c.y) ? b.y : c.y),
            (a.z < b.z) ? ((a.z < c.z) ? a.z : c.z) : ((b.z < c.z) ? b.z : c.z)
        ),
        vec3(
            (a.x > b.x) ? ((a.x > c.x) ? a.x : c.x) : ((b.x > c.x) ? b.x : c.x),
            (a.y > b.y) ? ((a.y > c.y) ? a.y : c.y) : ((b.y > c.y) ? b.y : c.y),
            (a.z > b.z) ? ((a.z > c.z) ? a.z : c.z) : ((b.z > c.z) ? b.z : c.z)
        )
    );
}