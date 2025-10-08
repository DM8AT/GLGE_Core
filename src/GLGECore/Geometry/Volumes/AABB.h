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
//include volumes
#include "Volume.h"

//include C++ vectors for utility
#if __cplusplus
    #include <vector>
#endif

//define a structure for axis aligned bounding boxes
typedef struct s_AABB
#if __cplusplus //for C++ add the volume class as a base class
final : public Volume
#endif
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
    s_AABB(const std::vector<vec3>& positions) noexcept {
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
    s_AABB(vec3* positions, size_t posCount) noexcept {
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
    s_AABB(const vec3& _min, const vec3& _max)
     : min(_min), max(_max)
    {}

    /**
     * @brief Construct a new AABB
     */
    s_AABB()
     : min(0), max(0)
    {}

    /**
     * @brief Get the Volume of the axis aligned bounding box
     * 
     * @return float the volume of the axis aligned bounding box
     */
    virtual float getVolume() const noexcept final override {return (max.x - min.x) * (max.y - min.y) * (max.z - min.z);}

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