/**
 * @file LayerStack.h
 * @author DM8AT
 * @brief define a layer stack system to collect and centralize layers
 * @version 0.1
 * @date 2025-09-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_LAYER_STACK_
#define _GLGE_CORE_LAYER_STACK_

//include the C/C++ layer system
#include "Layer.h"
//include the C++ layer system
#include "LayerBase.hpp"

//add the C binding vectors
#include "../../GLGE_BG/CBinding/Vector.h"

/**
 * @brief a single element that can be stored in a layer stack. It is used for C / C++ compatibility. 
 */
typedef struct s_LayerStackElement {
    //store if the layer is a C++ specific layer base
    bool isLayerBase;
    //store a void pointer to whatever is stored
    void* layer;

    //for C++ add constructors
    #if __cplusplus

    /**
     * @brief Construct a new Layer Stack Element
     * 
     * @param _isLayerBase true : this is a C++ class derived from LayerBase | false : this is a pointer to an layer object
     * @param _layer a pointer to the layer
     */
    s_LayerStackElement(bool _isLayerBase, void* _layer)
     : isLayerBase(_isLayerBase), layer(_layer)
    {}

    /**
     * @brief Construct a new Layer Stack Element
     * 
     * @param _layer a pointer to the C/C++ layer to store
     */
    s_LayerStackElement(Layer* _layer)
     : isLayerBase(false), layer(_layer)
    {}

    /**
     * @brief Construct a new Layer Stack Element
     * 
     * @param _layer a pointer to the class derived from the layer base class
     */
    s_LayerStackElement(LayerBase* _layer)
     : isLayerBase(true), layer(_layer)
    {}

    #endif

} LayerStackElement;

//for C++ create a class
#if __cplusplus

//std vectors are used for the stack inputs
#include <vector>

/**
 * @brief store a stack of all bound layers
 */
class LayerStack
{
public:

    /**
     * @brief Construct a new Layer Stack
     * 
     * @param layers a std::vector containing only pointer to layers
     */
    LayerStack(const std::vector<Layer*>& layers);

    /**
     * @brief Construct a new Layer Stack
     * 
     * @param layers a std::vector containing only pointer to classes that inherit from LayerBase
     */
    LayerStack(const std::vector<LayerBase*>& layers);

    /**
     * @brief Construct a new Layer Stack 
     * 
     * @param layers a std::vector containing the compatibility elements for the layer stack
     */
    LayerStack(const std::vector<LayerStackElement>& layers);

    /**
     * @brief Construct a new Layer Stack
     * 
     * @param layers a vector containing LayerBase*, Layer* or LayerStackElements. LayerStackElements are determined using the size and the pointers are differentiated by the boolean flag. 
     * @param isLayerBase a boolean to say tha type of pointer used
     */
    LayerStack(const Vector& layers, bool isLayerBase);

    /**
     * @brief Destroy the Layer Stack
     */
    ~LayerStack();

    /**
     * @brief a function to signal a startup to all layers
     */
    void signalStartup();

    /**
     * @brief a function to signal an update 
     * 
     */
    void update();

    /**
     * @brief a function to signal the shutdown of the application
     */
    void signalShutdown();

    /**
     * @brief Get the Element by a layer type
     * 
     * @param library the library the layer belongs to
     * @param identifier the identifier of the layer
     * @return const LayerStackElement* a constant pointer to the layer or NULL if it was not found
     */
    const LayerStackElement* getElement(const char* library, const char* identifier) const noexcept;

    /**
     * @brief Get the Element by an index
     * 
     * @param index the index of the layer to quarry
     * @return const LayerStackElement* a constant pointer to the layer or NULL if it was not found
     */
    const LayerStackElement* getElement(size_t index) const noexcept;

    /**
     * @brief get the element by an index
     * 
     * @param index the index of the layer to quarry
     * @return const LayerStackElement* a constant pointer to the layer or NULL if it was not found
     */
    inline const LayerStackElement* operator[](size_t index) const noexcept {return getElement(index);}

    /**
     * @brief add a new layer to the layer stack
     * 
     * @warning to add the layer, the combination of library and name must be unique in the stack
     * 
     * @param layer a pointer to the layer to add
     * @return true : the layer was added successfully
     * @return false : failed to add the layer
     */
    bool addNewLayer(Layer* layer);

    /**
     * @brief add a new layer to the layer stack
     * 
     * @warning to add the layer, the combination of library and name must be unique in the stack
     * 
     * @param layer a pointer to the new layer to add
     * @return true : the layer was added successfully
     * @return false : failed to add the layer
     */
    bool addNewLayer(LayerBase* layer);

    /**
     * @brief add a new layer to the layer stack
     * 
     * @warning to add the layer, the combination of library and name must be unique in the stack
     * 
     * @param layer a constant reference to a layer stack element to add to the layer stack
     * @return true : the layer was added successfully
     * @return false : failed to add the layer
     */
    bool addNewLayer(const LayerStackElement& layer);

    /**
     * @brief Get the index of a specific Layer 
     * 
     * @param library the name of the library to quarry the index for
     * @param name the name of the layer to quarry the index for
     * @return size_t the index or SIZE_MAX if it was not found
     */
    size_t getLayerIndex(const char* library, const char* name) const noexcept;

    /**
     * @brief remove a layer using an index
     * 
     * @param index the index of the layer to remove
     * 
     * @return true : the layer was removed successfully
     * @return false : failed to remove the layer
     */
    bool removeLayer(size_t index);

    /**
     * @brief remove a layer using a name
     * 
     * @param library the name of the library of the layer to remove
     * @param name the name of the layer to 
     * 
     * @return true : the layer was removed successfully
     * @return false : failed to remove the layer
     */
    bool removeLayer(const char* library, const char* name);

protected:

    /**
     * @brief store all layers of the stack
     */
    Vector m_layers;
    /**
     * @brief store if a startup signal was given
     */
    bool m_isStarted = false;

};

#else //else, create a C binding

/**
 * @brief define an opaque structure to use as a layer stack
 * 
 * @warning do NOT de-reference the structure in any case, the byte is truly unused and just some junk for MSVC to compile the structure
 */
typedef struct s_LayerStack {byte unused;} LayerStack;

#endif

//start a C section for C++
#if __cplusplus
extern "C" {
#endif

/**
 * @brief create a new layer stack instance
 * 
 * @param layers a pointer to an array of pointers to the actual layers
 * @param layerCount the amount of elements in the array
 * @return LayerStack* a pointer to the new layer stack instance
 */
LayerStack* layerStack_CreateFromLayers(Layer** layers, uint64_t layerCount);

/**
 * @brief create a new layer stack instance
 * 
 * @param layers a pointer to an array of pointers to classes that inherit from the LayerBase class
 * @param layerCount the amount of elements in the array
 * @return LayerStack* a pointer to the new layer stack instance
 */
LayerStack* layerStack_CreateFromLayerBases(LayerBase** layers, uint64_t layerCount);

/**
 * @brief create a new layer stack instance
 * 
 * @param layers a pointer to an array of layer stack elements
 * @param layerCount the amount of elements in the array
 * @return LayerStack* a pointer to the new layer stack instance
 */
LayerStack* layerStack_CreateFromLayerStackElements(LayerStackElement* layers, uint64_t layerCount);

/**
 * @brief create a new layer stack instance
 * 
 * @warning the LayerStackElement and the pointers are differentiated by the size associated with the objects
 * 
 * @param layers a pointer to an vector that contains either Layer*, LayerBase* or LayerStackElements. 
 * @param isLayerBase a boolean flag to indicate if the vector contains Layer* or LayerBase*. Is ignored if the vector contains LayerStackElements. 
 * @return LayerStack* a pointer to the new layer stack instance
 */
LayerStack* layerStack_CreateFromVector(const Vector* layers, bool isLayerBase);

/**
 * @brief delete an instance of the layer stack class
 * 
 * @param ls a pointer to the layer stack class to delete
 */
void layerStack_Delete(LayerStack* ls);

/**
 * @brief a function to signal a startup to all layers
 * 
 * @param ls a pointer to the layer stack to execute the function on
 */
void layerStack_SignalStartup(LayerStack* ls);

/**
 * @brief a function to signal an update 
 * 
 * @param ls a pointer to the layer stack to execute the function on
 */
void layerStack_Update(LayerStack* ls);

/**
 * @brief a function to signal the shutdown of the application
 * 
 * @param ls a pointer to the layer stack to execute the function on
 */
void layerStack_SignalShutdown(LayerStack* ls);

/**
 * @brief Get the Element by a layer type
 * 
 * @param library the library the layer belongs to
 * @param identifier the identifier of the layer
 * @param ls a pointer to the layer stack to execute the function on
 * @return const LayerStackElement* a constant pointer to the layer or NULL if it was not found
 */
const LayerStackElement* layerStack_GetElement(const char* library, const char* identifier, LayerStack* ls);

/**
 * @brief Get the Element by an index
 * 
 * @param index the index of the layer to quarry
 * @param ls a pointer to the layer stack to execute the function on
 * @return const LayerStackElement* a constant pointer to the layer or NULL if it was not found
 */
const LayerStackElement* layerStack_GetElementIndexed(size_t index, LayerStack* ls);

/**
 * @brief add a new layer to the layer stack
 * 
 * @warning to add the layer, the combination of library and name must be unique in the stack
 * 
 * @param layer a pointer to the layer to add
 * @param ls a pointer to the layer stack to execute the function on
 * @return true : the layer was added successfully
 * @return false : failed to add the layer
 */
bool layerStack_AddNewLayer_Layer(Layer* layer, LayerStack* ls);

/**
 * @brief add a new layer to the layer stack
 * 
 * @warning to add the layer, the combination of library and name must be unique in the stack
 * 
 * @param layer a pointer to the new layer to add
 * @param ls a pointer to the layer stack to execute the function on
 * @return true : the layer was added successfully
 * @return false : failed to add the layer
 */
bool layerStack_AddNewLayer_LayerBase(LayerBase* layer, LayerStack* ls);

/**
 * @brief add a new layer to the layer stack
 * 
 * @warning to add the layer, the combination of library and name must be unique in the stack
 * 
 * @param layer a constant reference to a layer stack element to add to the layer stack
 * @param ls a pointer to the layer stack to execute the function on
 * @return true : the layer was added successfully
 * @return false : failed to add the layer
 */
bool layerStack_AddNewLayer_LayerStackElement(LayerStackElement lse, LayerStack* ls);

/**
 * @brief Get the index of a specific Layer 
 * 
 * @param library the name of the library to quarry the index for
 * @param name the name of the layer to quarry the index for
 * @param ls a pointer to the layer stack to execute the function on
 * @return size_t the index or SIZE_MAX if it was not found
 */
size_t layerStack_GetLayerIndex(const char* library, const char* name, LayerStack* ls);

/**
 * @brief remove a layer using an index
 * 
 * @param index the index of the layer to remove
 * @param ls a pointer to the layer stack to execute the function on
 * 
 * @return true : the layer was removed successfully
 * @return false : failed to remove the layer
 */
bool layerStack_RemoveLayerIndexed(size_t index, LayerStack* ls);

/**
 * @brief remove a layer using a name
 * 
 * @param library the name of the library of the layer to remove
 * @param name the name of the layer to 
 * @param ls a pointer to the layer stack to execute the function on
 * 
 * @return true : the layer was removed successfully
 * @return false : failed to remove the layer
 */
bool layerStack_RemoveLayer(const char* library, const char* name, LayerStack* ls);

//end a C section for C++
#if __cplusplus
}
#endif

#endif