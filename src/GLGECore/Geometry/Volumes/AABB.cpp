/**
 * @file AABB.cpp
 * @author DM8AT
 * @brief implement the functions / the C interface
 * @version 0.1
 * @date 2025-10-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include AABBs
#include "AABB.h"

void aabb_Create(vec3* positions, uint64_t posCount, AABB* aabb) {*aabb = AABB(positions, posCount);}