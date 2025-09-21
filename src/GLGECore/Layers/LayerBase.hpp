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
     * 
     * @param type the type of the layer to create
     * @param func an optional callback function for a function to handle events
     * @param userData optional user data that would be passed to the event handling function
     * @param deferEvents if events can be handled, say if events should be handled immediately or at a later point in time
     */
    LayerBase(const LayerType& _type, void (*func)(const Event*, void*) = NULL, void* userData = NULL, bool deferEvents = false)
     : m_type(_type), m_handler(func, userData, deferEvents)
    {}

    /**
     * @brief Construct a new Layer Base
     * 
     * @param library the name of the library the layer belongs to
     * @param name tha name of the layer
     * @param func an optional callback function for a function to handle events
     * @param userData optional user data that would be passed to the event handling function
     * @param deferEvents if events can be handled, say if events should be handled immediately or at a later point in time
     */
    LayerBase(const char* library, const char* name, void (*func)(const Event*, void*) = NULL, void* userData = NULL, bool deferEvents = false)
     : m_type(library, name), m_handler(func, userData, deferEvents)
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

    /**
     * @brief Get the Event Handler contained by the layer
     * 
     * @return const EventHandler& a constant reference to the encapsulated handler
     */
    inline EventHandler& getEventHandler() noexcept {return m_handler;}

protected:

    /**
     * @brief store the type of layer
     */
    LayerType m_type;

    /**
     * @brief store an event handler
     */
    EventHandler m_handler = EventHandler(NULL, NULL);

};

#endif

#endif