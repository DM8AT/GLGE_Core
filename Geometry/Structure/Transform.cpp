/**
 * @file Transform.cpp
 * @author DM8AT
 * @brief implement the C binding for the transform
 * @version 0.1
 * @date 2025-10-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the transform API
#include "Transform.h"

mat4 transform_GetPositionMatrix(const Transform* transform) {return transform->getPositionMatrix();}

mat4 transform_GetRotationMatrix(const Transform* transform) {return transform->getRotationMatrix();}

mat4 transform_GetScaleMatrix(const Transform* transform) {return transform->getScaleMatrix();}

mat4 transform_GetTransformMatrix(const Transform* transform) {return transform->getTransformMatrix();}
