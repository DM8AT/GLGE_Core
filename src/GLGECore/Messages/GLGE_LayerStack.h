/**
 * @file GLGE_LayerStack.h
 * @author DM8AT
 * @brief define a layer stack interface for C and C++
 * @version 0.1
 * @date 2025-09-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_C_CXX_MESSAGES_LAYER_STACK_
#define _GLGE_CORE_C_CXX_MESSAGES_LAYER_STACK_

//include the C vector binding
#include "../../GLGE_BG/CBinding/Vector.h"

//include layers
#include "GLGE_Layers.h"

//check for C++ to create a class
#if __cplusplus

class LayerStack
{
public:

    LayerStack() = default;

protected:

    //store a vector with all the layers of the layer stack
    Vector m_layers = Vector(sizeof(CompatLayer));

};

#endif

#endif