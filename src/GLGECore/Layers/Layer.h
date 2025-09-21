/**
 * @file Layer.h
 * @author DM8AT
 * @brief define the API for a layer that can be used in C and C++
 * @version 0.1
 * @date 2025-09-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_LAYER_
#define _GLGE_CORE_LAYER_

//include all types
#include "../Types.h"
//include the event system
#include "../Events/Events.h"

/**
 * @brief define a structure to identify layers
 */
typedef struct s_LayerType {
    //store the library the layer belongs to
    const char* library;
    //store an individual layer name
    const char* name;

    //for C++ create a constructor
    #if __cplusplus

    /**
     * @brief Construct a new Layer Type
     * 
     * @param _library the name of the library the layer belongs to
     * @param _name the name of the layer
     */
    s_LayerType(const char* _library, const char* _name)
     : library(_library), name(_name)
    {}

    #endif

} LayerType;

/**
 * @brief define a structure to store all callback functions
 */
typedef struct s_LayerCallbacks {
    //store a pointer to a structure that extends this structure
    //the API is inspired by the vulkan's next system
    void* next;
    //store a function to call for each update
    void (*func_updateCallback)();
    //a function to call for the app startup
    void (*func_startupCallback)();
    //a function to call for the app shutdown
    void (*func_shutdownCallback)();
} LayerCallbacks;

/**
 * @brief define a structure to store if a layer supports events and if it does how to handle them
 */
typedef struct s_LayerEvents {
    //store if the layer supports events
    bool supportsEvents;
    //store an optional variable tha only needs to be initialized if supportsEvents is true
    EventHandler handler;

    //for C++ add constructors
    #if __cplusplus

    /**
     * @brief Construct a new event layer structure
     * 
     * This layer won't receive any events
     */
    s_LayerEvents()
     : supportsEvents(false), handler(NULL, NULL, false)
    {}

    /**
     * @brief Construct a new event layer structure
     * 
     * @param handleFunc a function to handle the events
     * @param userData some user data to pass to the function to call
     * @param deferEvents true : events will be deferred by default | false : events will be handled instantly
     */
    s_LayerEvents(void (*handleFunc)(const Event*, void*), void* userData, bool deferEvents = false)
     : supportsEvents(true), handler(handleFunc, userData, deferEvents)
    {}

    #endif

} LayerEvents;

/**
 * @brief store a layer of the application
 */
typedef struct s_Layer {
    //store the type of layer
    LayerType type;
    //store the event system
    LayerEvents events;
    //store the callback functions for the layer
    LayerCallbacks callbacks;

    //for C++ create a constructor
    #if __cplusplus

    /**
     * @brief Construct a new Layer
     * 
     * @param _type the type of the layer to create
     * @param _callbacks the callback functions the layer has
     */
    s_Layer(const LayerType& _type, const LayerCallbacks& _callbacks)
     : type(_type), events(), callbacks(_callbacks)
    {}

    /**
     * @brief Construct a new Layer
     * 
     * @param _type the type of the layer to create
     * @param handleFunc a function to handle the events
     * @param userData some user data to pass to the function to call
     * @param _callbacks the callback functions the layer has
     */
    s_Layer(const LayerType& _type, void (*handleFunc)(const Event*, void*), void* userData, const LayerCallbacks& _callbacks)
     : type(_type), events(handleFunc, userData), callbacks(_callbacks)
    {}

    /**
     * @brief Construct a new Layer
     * 
     * @param _type the type of the layer to create
     * @param handleFunc a function to handle the events
     * @param userData some user data to pass to the function to call
     * @param deferEvents true : events will be deferred by default | false : events will be handled instantly
     * @param _callbacks the callback functions the layer has
     */
    s_Layer(const LayerType& _type, void (*handleFunc)(const Event*, void*), void* userData, bool deferEvents, const LayerCallbacks& _callbacks)
     : type(_type), events(handleFunc, userData, deferEvents), callbacks(_callbacks)
    {}

    /**
     * @brief Construct a new Layer 
     * 
     * @param library the name fo the library the layer belongs to
     * @param name the name of the layer
     * @param _callbacks the callback functions the layer has
     */
    s_Layer(const char* library, const char* name, const LayerCallbacks& _callbacks)
     : type(library, name), events(), callbacks(_callbacks)
    {}

    /**
     * @brief Construct a new Layer 
     * 
     * @param library the name fo the library the layer belongs to
     * @param name the name of the layer
     * @param handleFunc a function to handle the events
     * @param userData some user data to pass to the function to call
     * @param _callbacks the callback functions the layer has
     */
    s_Layer(const char* library, const char* name, void (*handleFunc)(const Event*, void*), void* userData, const LayerCallbacks& _callbacks)
     : type(library, name), events(handleFunc, userData), callbacks(_callbacks)
    {}

    /**
     * @brief Construct a new Layer 
     * 
     * @param library the name fo the library the layer belongs to
     * @param name the name of the layer
     * @param handleFunc a function to handle the events
     * @param userData some user data to pass to the function to call
     * @param deferEvents true : events will be deferred by default | false : events will be handled instantly
     * @param _callbacks the callback functions the layer has
     */
    s_Layer(const char* library, const char* name, void (*handleFunc)(const Event*, void*), void* userData, bool deferEvents, const LayerCallbacks& _callbacks)
     : type(library, name), events(handleFunc, userData, deferEvents), callbacks(_callbacks)
    {}

    #endif

} Layer;

#endif