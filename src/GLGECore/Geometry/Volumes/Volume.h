/**
 * @file Volume.h
 * @author DM8AT
 * @brief define an abstract class to provide combined functionality for all volumes
 * @version 0.1
 * @date 2025-10-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_GEOMETRY_VOLUMES_VOLUME_
#define _GLGE_CORE_GEOMETRY_VOLUMES_VOLUME_

//only define the class for C++
#if __cplusplus

/**
 * @brief define a class to define a combined interface for volumes
 */
class Volume
{
public:

    /**
     * @brief Construct a new Volume
     */
    Volume() = default;

    /**
     * @brief Destroy the Volume
     */
    virtual ~Volume() = default;

    /**
     * @brief Get the volume of the volume structure
     * 
     * @return float a float for the volume
     */
    virtual float getVolume() const noexcept = 0;

};

#endif

//add C functions for the virtual functions

/**
 * @brief get the volume of a volume structure
 * 
 * @param volume a pointer to the volume overloaded to quarry the volume from
 * @return float the volume of the volume object
 */
float volume_GetVolume(Volume* volume);

#endif