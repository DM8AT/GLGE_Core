/**
 * @file Volume.cpp
 * @author DM8AT
 * @brief implement the C binding for the volumes
 * @version 0.1
 * @date 2025-10-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the volume API
#include "Volume.h"

float volume_GetVolume(Volume* volume) {return volume->getVolume();}