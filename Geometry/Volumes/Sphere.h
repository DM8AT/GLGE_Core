/**
 * @file Sphere.h
 * @author DM8AT
 * @brief define the volumes for spheres
 * @version 0.1
 * @date 2025-10-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_GEOMETRY_VOLUMES_SPHERE_
#define _GLGE_CORE_GEOMETRY_VOLUMES_SPHERE_

//include math types
#include "../../../GLGE_Math/GLGEMath.h"

/**
 * @brief store a simple sphere
 */
typedef struct s_Sphere
{
    //the position of the sphere
    vec3 pos;
    //the radius of the sphere
    float radius;

    //define the C++ functions
    #if __cplusplus

    /**
     * @brief Construct a new Sphere
     */
    inline constexpr s_Sphere() noexcept
     : pos(0), radius(0)
    {}

    /**
     * @brief Construct a new Sphere
     * 
     * @param _pos the position of the sphere
     * @param _radius the radius of the sphere
     */
    inline constexpr s_Sphere(const vec3& _pos, const float _radius) noexcept
     : pos(_pos), radius(_radius)
    {}

    /**
     * @brief Get the Volume of the sphere
     * 
     * @return float the volume of the sphere
     */
    inline constexpr float getVolume() const noexcept {return (4.f * 3.141592f * (radius*radius*radius)) * (1.f/3.f);}
    
    /**
     * @brief add a point to the sphere volume
     * 
     * @param point the position of the point to add to the sphere
     */
    inline void merge(const vec3& point) noexcept
    {
        //get a vector from the center to the point and get the length
        vec3 centerToPoint = point - pos;
        float dist = length(centerToPoint);

        //if point is already inside or on the sphere, no need to change anything
        if (dist <= radius) return;

        //otherwise, expand the sphere radius to include the new point
        radius = (radius + dist) * 0.5f;

        //shift the center towards the new point accordingly
        float shift = radius - dist;
        if (dist > 0.f)
        {
            pos += (centerToPoint / dist) * shift;
        }
    }

    /**
     * @brief Merge this sphere with another to form a tight bounding sphere
     * 
     * @param other the other sphere to merge with
     */
    inline void merge(const s_Sphere& other) noexcept
    {
        //compute a vector from one center to another and calculate the length of the vector
        const vec3 centerVec = other.pos - pos;
        const float dist = length(centerVec);

        //If one sphere is fully inside the other, just expand if needed
        if (radius >= dist + other.radius) {return;}

        //just use the other sphere (this sphere is fully contained)
        if (other.radius >= dist + radius)
        {
            //store the other sphere as this one
            pos = other.pos;
            radius = other.radius;
            return;
        }

        //compute the new radius
        const float newRadius = (dist + radius + other.radius) * 0.5f;
        //calculate by how much the center needs to be shifted
        const float shift = newRadius - radius;

        //store the new position
        if (dist > 0.f) {pos += (centerVec / dist) * shift;}

        //store the new radius (don't forget this)
        radius = newRadius;
    }

    #endif

} Sphere;

#endif