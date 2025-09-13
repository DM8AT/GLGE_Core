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

//include std::vectors to simplify construction
#include <vector>

class LayerStack
{
public:

    /**
     * @brief Construct a new Layer Stack
     * 
     * The layer stack will be empty
     */
    LayerStack() = default;

    /**
     * @brief Construct a new Layer Stack
     * 
     * @param layers a constant reference to an std::vector containing all the C++ layers for the layer stack
     */
    LayerStack(const std::vector<CppLayer*>& layers);

    /**
     * @brief Construct a new Layer Stack
     * 
     * @param layers a constant reference to an std::vector containing all the C layers for the layer stack
     */
    LayerStack(const std::vector<Layer>& layers);

    /**
     * @brief Construct a new Layer Stack
     * 
     * @param layers a constant reference to an std::vector containing all the C / C++ layers for the layer stack
     */
    LayerStack(const std::vector<CompatLayer>& layers);

protected:

    //store a vector with all the layers of the layer stack
    Vector m_layers = Vector(sizeof(CompatLayer));

};

#endif

#endif