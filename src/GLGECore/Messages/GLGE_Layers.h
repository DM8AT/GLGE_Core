/**
 * @file GLGE_Layers.hpp
 * @author DM8AT
 * @brief define a C/C++ interface for the GLGE layer system
 * @version 0.1
 * @date 2025-09-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_CXX_MESSAGES_LAYERS_
#define _GLGE_CORE_CXX_MESSAGES_LAYERS_

//include the messages
#include "GLGE_Message.h"

//check for C++ to create a class
#if __cplusplus

/**
 * @brief define the virtual class to handle layers
 */
class CppLayer
{
public:
    /**
     * @brief handle a single message
     * 
     * @param msg a constant reference to the message to handle
     * @return true : the message was handled and will not treverse the layer stack any further
     * @return false : the message MAY BE HANDLED and will continue to treverse the layer stack
     */
    virtual bool evaluateMessage(const Message& msg) noexcept {return false;}
};

#endif

//check for C++ to start a C section
#if __cplusplus
extern "C" {
#endif

//else, create a C structure
typedef struct s_Layer {
    /**
     * @brief handle a single message
     * 
     * @param msg a constant reference to the message to handle
     * @return true : the message was handled and will not treverse the layer stack any further
     * @return false : the message MAY BE HANDLED and will continue to treverse the layer stack
     */
    bool (*evaluateMessage)(const Message* msg);
} Layer;

//check for C++ to end a C section
#if __cplusplus
}

//also, create a unified version of the Layer and CppLayer structure
struct CompatLayer {
    //store if a C++ layer is stored
    bool isCpp = false;

    //store eather a C++ or a C layer
    union {
        //store the C layer
        Layer c_layer;
        //store the C++ layer
        CppLayer* cpp_layer;
    };
};

#endif

#endif