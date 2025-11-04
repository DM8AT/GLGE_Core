/**
 * @file AABB.h
 * @author DM8AT
 * @brief define the API for axis aligned bounding boxes
 * @version 0.1
 * @date 2025-10-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_GEOMETRY_VOLUMES_AABB_
#define _GLGE_CORE_GEOMETRY_VOLUMES_AABB_

//include vectors
#include "../../../GLGE_Math/GLGEMath.h"

//include C++ vectors for utility
#if __cplusplus
    #include <vector>
#endif

//define a structure for axis aligned bounding boxes
typedef struct s_AABB
{
    //store the minimum values for the box
    vec3 min;
    //store the maximum values for the box
    vec3 max;

    //define functions for C++
    #if __cplusplus

    /**
     * @brief Construct a new AABB
     * 
     * @param positions a list of positions to create the AABB from
     */
    inline constexpr s_AABB(const std::vector<vec3>& positions) noexcept {
        for (size_t i = 0; i < positions.size(); ++i) {
            //store the per-axis minimum
            min.x = (min.x > positions[i].x) ? positions[i].x : min.x;
            min.y = (min.y > positions[i].y) ? positions[i].y : min.y;
            min.z = (min.z > positions[i].z) ? positions[i].z : min.z;
            //store the per-axis maximum
            max.x = (max.x < positions[i].x) ? positions[i].x : max.x;
            max.y = (max.y < positions[i].y) ? positions[i].y : max.y;
            max.z = (max.z < positions[i].z) ? positions[i].z : max.z;
        }
    }

    /**
     * @brief Construct a new AABB
     * 
     * @param positions a C array of 3D float vectors for positions
     * @param posCount the amount of positions in the C array
     */
    inline constexpr s_AABB(vec3* positions, size_t posCount) noexcept {
        for (size_t i = 0; i < posCount; ++i) {
            //store the per-axis minimum
            min.x = (min.x > positions[i].x) ? positions[i].x : min.x;
            min.y = (min.y > positions[i].y) ? positions[i].y : min.y;
            min.z = (min.z > positions[i].z) ? positions[i].z : min.z;
            //store the per-axis maximum
            max.x = (max.x < positions[i].x) ? positions[i].x : max.x;
            max.y = (max.y < positions[i].y) ? positions[i].y : max.y;
            max.z = (max.z < positions[i].z) ? positions[i].z : max.z;
        }
    }

    /**
     * @brief Construct a new AABB
     * 
     * @param _min the value for the minim values
     * @param _max the values for the maximum values
     */
    inline constexpr s_AABB(const vec3& _min, const vec3& _max) noexcept
     : min(_min), max(_max)
    {}

    /**
     * @brief Construct a new AABB
     */
    inline constexpr s_AABB() noexcept
     : min(0), max(0)
    {}

    /**
     * @brief Get the Volume of the axis aligned bounding box
     * 
     * @return float the volume of the axis aligned bounding box
     */
    inline constexpr float getVolume() const noexcept {return (max.x - min.x) * (max.y - min.y) * (max.z - min.z);}

    /**
     * @brief add a single point to the AABB
     * 
     */
    inline constexpr void merge(const vec3& pos) noexcept {
        //store the new minimum
        min.x = (min.x < pos.x) ? min.x : pos.x;
        min.y = (min.y < pos.y) ? min.y : pos.y;
        min.z = (min.z < pos.z) ? min.z : pos.z;
        //store the new maximum
        max.x = (max.x > pos.x) ? max.x : pos.x;
        max.y = (max.y > pos.y) ? max.y : pos.y;
        max.z = (max.z > pos.z) ? max.z : pos.z;
    }

    /**
     * @brief add another AABB to this one by including it
     * 
     * @param aabb a constant reference to the AABB to merge with this one
     */
    inline constexpr void merge(const s_AABB& aabb) noexcept {
        //store the new minimum
        min.x = (min.x < aabb.min.x) ? min.x : aabb.min.x;
        min.y = (min.y < aabb.min.y) ? min.y : aabb.min.y;
        min.z = (min.z < aabb.min.z) ? min.z : aabb.min.z;
        //store the new maximum
        max.x = (max.x > aabb.max.x) ? max.x : aabb.max.x;
        max.y = (max.y > aabb.max.y) ? max.y : aabb.max.y;
        max.z = (max.z > aabb.max.z) ? max.z : aabb.max.z;
    }

    /**
     * @brief print an axis aligned bounding box into an output stream
     * 
     * @param os the output stream to print to
     * @param aabb the axis aligned bounding box to print
     * @return std::ostream& the filled output stream
     */
    inline friend std::ostream& operator<<(std::ostream& os, const s_AABB& aabb) noexcept {
        return os << "{min: " << aabb.min << " | max: " << aabb.max << "}";
    }

    #endif

} AABB;

//add the C interface functions

/**
 * @brief create a new axis aligned bounding box
 * 
 * @param positions an array of 3D float vectors for the positions
 * @param posCount the amount of positions in the array
 * @param aabb a pointer to the axis aligned bounding box to fill out
 */
void aabb_Create(vec3* positions, uint64_t posCount, AABB* aabb);

#endif