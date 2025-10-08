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

//include volumes
#include "Volume.h"
//include math types
#include "../../../GLGE_Math/GLGEMath.h"

typedef struct s_Sphere
#if __cplusplus
final : public Volume
#endif
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
    s_Sphere()
     : pos(0), radius(0)
    {}

    /**
     * @brief Construct a new Sphere
     * 
     * @param _pos the position of the sphere
     * @param _radius the radius of the sphere
     */
    s_Sphere(const vec3& _pos, const float _radius)
     : pos(_pos), radius(_radius)
    {}

    /**
     * @brief Get the Volume of the sphere
     * 
     * @return float the volume of the sphere
     */
    inline virtual float getVolume() const noexcept final override {return (4.f * M_PIf * (radius*radius*radius)) * (1.f/3.f);}

    #endif

} Sphere;

#endif