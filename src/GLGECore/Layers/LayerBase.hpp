/**
 * @file LayerBase.hpp
 * @author DM8AT
 * @brief define a C++ only API for layers to use them like a base class
 * @version 0.1
 * @date 2025-09-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_LAYER_BASE_
#define _GLGE_CORE_LAYER_BASE_

//include C/C++ layers
#include "Layer.h"

//only available for C++
#if __cplusplus

/**
 * @brief define a virtual base class for the layer
 */
class LayerBase
{
public:

    /**
     * @brief Construct a new Layer Base
     */
    LayerBase(const LayerType& _type)
     : m_type(_type)
    {}

    LayerBase(const char* library, const char* name)
     : m_type(library, name)
    {}

    /**
     * @brief Destroy the Layer Base 
     */
    virtual ~LayerBase() {}

    /**
     * @brief a virtual function that is called once on startup
     */
    virtual void onSetup() {}

    /**
     * @brief a virtual function that is called once per tick
     */
    virtual void onUpdate() {}

    /**
     * @brief a virtual function that is called on shutdown
     */
    virtual void onShutdown() {}

    /**
     * @brief Get the Type of layer this is
     * 
     * @return const LayerType& a constant reference to the layer type that stores the layer's type
     */
    inline const LayerType& getType() const noexcept {return m_type;}

protected:

    /**
     * @brief store the type of layer
     */
    LayerType m_type;

};

#endif

#endif