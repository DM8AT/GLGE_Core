/**
 * @file Triangle.h
 * @author DM8AT
 * @brief define what a triangle is
 * @version 0.1
 * @date 2025-10-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_GEOMETRY_SURFACE_TRIANGLE_
#define _GLGE_CORE_GEOMETRY_SURFACE_TRIANGLE_

//include vector types
#include "../../../GLGE_Math/GLGEMath.h"

/**
 * @brief define what a triangle is
 */
typedef struct s_Triangle {
    //store three 3D float vectors for the position of the vertices
    vec3 a, b, c;

    //for C++ add utility functions
    #if __cplusplus

    /**
     * @brief Construct a new Triangle
     */
    constexpr s_Triangle() = default;

    /**
     * @brief Construct a new Triangle
     * 
     * @param _a the value for the first vertex position
     * @param _b the value for the second vertex position
     * @param _c the value for the third vertex position
     */
    constexpr s_Triangle(const vec3& _a, const vec3& _b, const vec3& _c)
     : a(_a), b(_b), c(_c)
    {}

    /**
     * @brief Get the Area of a triangle
     * 
     * @return float the area of the triangle
     */
    inline float getArea() const noexcept {return length(cross(a - b, a - c)) * 0.5f;}

    /**
     * @brief Get the Normal vector of the triangle
     * 
     * @return vec3 the normal vector of the triangle
     */
    inline vec3 getNormal() const noexcept {return normalize(cross(a - b, a - c));}

    /**
     * @brief print the triangle into an output stream
     * 
     * @param os the output stream to print to
     * @param tri the triangle to print
     * @return std::ostream& the filled output stream
     */
    inline friend std::ostream& operator<<(std::ostream& os, const s_Triangle& tri) noexcept {
        return os << "{" << tri.a << ", " << tri.b << ", " << tri.c << "}";
    }

    /**
     * @brief Get the Bounding Volume of the triangle
     * 
     * @tparam T the type of bounding volume to create
     * @return T the volume that contains the triangle
     */
    template <typename T> T getBoundingVolume() const noexcept;

    #endif

} Triangle;

#endif