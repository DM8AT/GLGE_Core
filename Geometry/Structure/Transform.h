/**
 * @file Transform.h
 * @author DM8AT
 * @brief a transform defines a 3D position, 3D rotation and 3D scale. 
 * @version 0.1
 * @date 2025-10-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_GEOMETRY_STRUCTURE_TRANSFORM_
#define _GLGE_CORE_GEOMETRY_STRUCTURE_TRANSFORM_

//include math stuff (like vectors and quaternions)
#include "../../../GLGE_Math/GLGEMath.h"

//define the structure of a transform object
typedef struct s_Transform {
    //store the position of the transformation
    vec3 pos;
    //store the rotation of the transformation
    Quaternion rot;
    //store the scale of the transformation
    vec3 scale;

    //for C++ add non-virtual member functions
    #if __cplusplus

    /**
     * @brief Construct a new Transform
     */
    inline s_Transform()
     : pos(0), rot(1,0,0,0), scale(1)
    {}

    /**
     * @brief Construct a new Transform
     * 
     * @param _pos the position of the transformation
     * @param _rot the rotation of the transformation
     * @param _scale the scale of the transformation
     */
    inline s_Transform(const vec3& _pos, const Quaternion& _rot = Quaternion(), const vec3& _scale = 1)
     : pos(_pos), rot(_rot), scale(_scale)
    {}

    /**
     * @brief Get the matrix to apply the position stored in the transform
     * 
     * @return constexpr mat4 the matrix to apply the transform
     */
    inline constexpr mat4 getPositionMatrix() const noexcept
    {
        //return a matrix to move the object around
        return mat4(
            1,0,0,pos.x,
            0,1,0,pos.y,
            0,0,1,pos.z,
            0,0,0,1
        );
    }

    /**
     * @brief Get a matrix to apply the rotation stored in the transform
     * 
     * @return mat4 a 4x4 matrix to apply the rotation of the transform
     */
    inline constexpr mat4 getRotationMatrix() const noexcept
    {
        //return a 4x4 matrix for the rotation created from the quaternion
        return mat4(
            1. - 2.*rot.j*rot.j - 2.*rot.k*rot.k, 2.*rot.i*rot.j - 2.*rot.k*rot.w, 2.*rot.i*rot.k + 2.*rot.j*rot.w, 0,
            2.*rot.i*rot.j + 2.*rot.k*rot.w, 1. - 2.*rot.i*rot.i - 2.*rot.k*rot.k, 2.*rot.j*rot.k - 2.*rot.i*rot.w, 0,
            2.*rot.i*rot.k - 2*rot.j*rot.w, 2*rot.j*rot.k + 2*rot.i*rot.w, 1. - 2.*rot.i*rot.i - 2.*rot.j*rot.j,    0,
            0, 0, 0, 1
        );
    }

    /**
     * @brief Get the Scale to scale the matrix according to the transform
     * 
     * @return mat4 a matrix to apply the scale of the transfrom
     */
    inline mat4 getScaleMatrix() const noexcept
    {
        //return the scale matrix
        return mat4(
            scale.x, 0, 0, 0,
            0, scale.y, 0, 0,
            0, 0, scale.z, 0,
            0, 0, 0,       1
        );
    }

    /**
     * @brief Get the matrix that applies the whole transformation
     * 
     * @return mat4 a matrix to apply the transformation with
     */
    inline mat4 getTransformMatrix() const noexcept
    {
        //return the output of multiplying the above matrices together in the following order:
        //(scale * rotation) * position
        return mat4(
            scale.x * (1. - 2.*rot.j*rot.j - 2.*rot.k*rot.k), scale.x * (2.*rot.i*rot.j - 2.*rot.k*rot.w),      scale.x * (2.*rot.i*rot.k + 2.*rot.j*rot.w),      pos.x,
            scale.y * (2.*rot.i*rot.j + 2.*rot.k*rot.w),      scale.y * (1. - 2.*rot.i*rot.i - 2.*rot.k*rot.k), scale.y * (2.*rot.j*rot.k - 2.*rot.i*rot.w),      pos.y,
            scale.z * (2.*rot.i*rot.k - 2*rot.j*rot.w),       scale.z * (2*rot.j*rot.k + 2*rot.i*rot.w),        scale.z * (1. - 2.*rot.i*rot.i - 2.*rot.j*rot.j), pos.z,
            0, 0, 0, 1
        );
    }

    #endif

} Transform;

//also add the C functions
#if __cplusplus
extern "C" {
#endif

/**
 * @brief get the positioning matrix from a transformation object
 * 
 * @param transform the transform object to get the position from
 * @return mat4 the positioning matrix
 */
mat4 transform_GetPositionMatrix(const Transform* transform);

/**
 * @brief get the rotation matrix from a transform object
 * 
 * @param transform the transform to quarry the rotation matrix from
 * @return mat4 the rotation matrix of the transform object
 */
mat4 transform_GetRotationMatrix(const Transform* transform);

/**
 * @brief get the scale matrix from a transform object
 * 
 * @param transform the transform to quarry the scale matrix from
 * @return mat4 the matrix to apply the scaling
 */
mat4 transform_GetScaleMatrix(const Transform* transform);

/**
 * @brief get the transformation matrix from a transform object
 * 
 * @param transform the transform object to quarry the transformation matrix from
 * @return mat4 a matrix to apply the transformation of the transform object
 */
mat4 transform_GetTransformMatrix(const Transform* transform);

//end the C section
#if __cplusplus
}
#endif

#endif