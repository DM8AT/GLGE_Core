/**
 * @file LayerStack.cpp
 * @author DM8AT
 * @brief implement the layer stack for C++ and the compatibility functions for C
 * @version 0.1
 * @date 2025-09-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//add the layer stacks
#include "LayerStack.h"
//include debugging stuff
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"
//include C string working stuff
#include <cstring>

/**
 * @brief Get the Layer Type from a layer stack element
 * 
 * @param el a constant reference to the layer stack element
 * @return LayerType the layer type
 */
inline static LayerType getLayerType(const LayerStackElement& el) noexcept
{
    //store the type to return
    LayerType type("", "");
    //check depends on the type of element
    if (el.isLayerBase)
    {
        //this is a C++ base class
        //get the layer identifier
        type = ((LayerBase*)el.layer)->getType();
    }
    else
    {
        //else, this is a C structure
        type = ((Layer*)el.layer)->type;
    }

    //return the type
    return type;
}

LayerStack::LayerStack(const std::vector<Layer*>& layers)
 : m_layers(sizeof(LayerStackElement), layers.size())
{
    //iterate over all elements to safety-check them and add them to the list
    for (size_t i = 0; i < layers.size(); ++i)
    {
        //add the new layer
        if (!addNewLayer(layers[i]))
        {GLGE_ABORT("Failed to add a layer to a layer stack");}
    }
}

LayerStack::LayerStack(const std::vector<LayerBase*>& layers)
 : m_layers(sizeof(LayerStackElement), layers.size())
{
    //iterate over all elements to safety-check them and add them to the list
    for (size_t i = 0; i < layers.size(); ++i)
    {
        //add the new layer
        if (!addNewLayer(layers[i]))
        {GLGE_ABORT("Failed to add a layer to a layer stack");}
    }
}

LayerStack::LayerStack(const std::vector<LayerStackElement>& layers)
 : m_layers(sizeof(LayerStackElement), layers.size())
{
    //iterate over all elements to safety-check them and add them to the list
    for (size_t i = 0; i < layers.size(); ++i)
    {
        //add the new layer
        if (!addNewLayer(layers[i]))
        {GLGE_ABORT("Failed to add a layer to a layer stack");}
    }
}

LayerStack::LayerStack(const Vector& layers, bool isLayerBase)
 : m_layers(sizeof(LayerStackElement), layers.byteSize / layers.elementSize)
{
    //iterate over all elements to safety-check them and add them to the list
    uint64_t size = layers.byteSize / layers.elementSize;
    //switch depending on the size of the elements
    if (layers.elementSize == sizeof(LayerStackElement))
    {
        //all elements are layer stack elements
        for (uint64_t i = 0; i < size; ++i)
        {
            //add the layer stack elements
            if (!addNewLayer(*(((LayerStackElement*)layers.ptr) + i)))
            {GLGE_ABORT("Failed to add a layer to a layer stack");}
        }
    }
    else if (layers.elementSize == sizeof(void*))
    {
        //switch depending on the inputted boolean
        if (isLayerBase)
        {
            //the vector contains base classes
            for (uint64_t i = 0; i < size; ++i)
            {
                if (!addNewLayer(((LayerBase*)layers.ptr) + i))
                {GLGE_ABORT("Failed to add a layer to a layer stack");}
            }
        }
        else
        {
            //the vector contains C/C++ layer structures
            for (uint64_t i = 0; i < size; ++i)
            {
                if (!addNewLayer(((Layer*)layers.ptr) + i))
                {GLGE_ABORT("Failed to add a layer to a layer stack");}
            }
        }
    }
    else
    {
        //invalid size
        GLGE_ABORT("Invalid size for a layer stack vector");
    }
}

LayerStack::~LayerStack()
{
    //if the element is currently started, call the shutdown
    if (m_isStarted) {signalShutdown();}

    //iterate over all layers
    uint64_t size = m_layers.byteSize / m_layers.elementSize;
    //clean the layers up
    for (uint64_t i = 0; i < size; ++i)
    {
        //get the element to clean up
        LayerStackElement& el = *(((LayerStackElement*)m_layers.ptr) + i);
        //call the correct destructor
        if (el.isLayerBase)
        {delete (LayerBase*)el.layer;}
        else
        {delete (Layer*)el.layer;}
        el.layer = NULL;
    }
    //clean up the vector
    m_layers.clear();
}

void LayerStack::signalStartup()
{
    //if the startup was allready called, stop
    if (m_isStarted) {return;}

    //iterate over all layers and call the startup function
    uint64_t layerCount = m_layers.byteSize / m_layers.elementSize;
    for (uint64_t i = 0; i < layerCount; ++i)
    {
        //get the layer
        LayerStackElement& el = *(((LayerStackElement*)m_layers.ptr) + i);
        //switch depending on the type of layer
        if (el.isLayerBase)
        {
            //call the virtual function
            ((LayerBase*)el.layer)->onSetup();
        }
        else
        {
            //get the function pointer
            void (*fptr)() = ((Layer*)el.layer)->callbacks.func_startupCallback;
            //if the function exists, call it
            if (fptr) {(*fptr)();}
        }
    }

    //startup was signaled
    m_isStarted = true;
}

void LayerStack::update()
{
    //iterate over all layers and call the startup function
    uint64_t layerCount = m_layers.byteSize / m_layers.elementSize;
    for (uint64_t i = 0; i < layerCount; ++i)
    {
        //get the layer
        LayerStackElement& el = *(((LayerStackElement*)m_layers.ptr) + i);
        //switch depending on the type of layer
        if (el.isLayerBase)
        {
            //call the virtual function
            ((LayerBase*)el.layer)->onUpdate();
        }
        else
        {
            //get the function pointer
            void (*fptr)() = ((Layer*)el.layer)->callbacks.func_updateCallback;
            //if the function exists, call it
            if (fptr) {(*fptr)();}
        }
    }
}

void LayerStack::signalShutdown()
{
    //if startup was not signaled, stop
    if (!m_isStarted) {return;}

    //iterate over all layers and call the startup function
    uint64_t layerCount = m_layers.byteSize / m_layers.elementSize;
    for (uint64_t i = 0; i < layerCount; ++i)
    {
        //get the layer
        LayerStackElement& el = *(((LayerStackElement*)m_layers.ptr) + i);
        //switch depending on the type of layer
        if (el.isLayerBase)
        {
            //call the virtual function
            ((LayerBase*)el.layer)->onShutdown();
        }
        else
        {
            //get the function pointer
            void (*fptr)() = ((Layer*)el.layer)->callbacks.func_shutdownCallback;
            //if the function exists, call it
            if (fptr) {(*fptr)();}
        }
    }

    //now, startup can be called again
    m_isStarted = false;
}

const LayerStackElement* LayerStack::getElement(const char* library, const char* identifier) const noexcept
{
    //search the index and return
    return getElement(getLayerIndex(library, identifier));
}

const LayerStackElement* LayerStack::getElement(size_t index) const noexcept
{
    //sanity check the index
    if (index * m_layers.elementSize >= m_layers.byteSize) {return NULL;}
    //return the pointer
    return ((LayerStackElement*)m_layers.ptr) + index;
}

bool LayerStack::addNewLayer(Layer* layer)
{
    //check if the layer is NOT found
    if (getLayerIndex(layer->type.library, layer->type.name) != SIZE_MAX) {return false;}

    //add the new layer
    LayerStackElement el = LayerStackElement(layer);
    m_layers.push_back(&el);

    //success
    return true;
}

bool LayerStack::addNewLayer(LayerBase* layer)
{
    //check if the layer is NOT found
    if (getLayerIndex(layer->getType().library, layer->getType().name) != SIZE_MAX) {return false;}

    //add the new layer
    LayerStackElement el = LayerStackElement(layer);
    m_layers.push_back(&el);

    //success
    return true;
}

bool LayerStack::addNewLayer(const LayerStackElement& layer)
{
    //get the layer type
    LayerType type = getLayerType(layer);

    //check if the layer is NOT found
    if (getLayerIndex(type.library, type.name) != SIZE_MAX) {return false;}

    //add the new layer
    m_layers.push_back((LayerStackElement*)&layer);

    //success
    return true;
}

size_t LayerStack::getLayerIndex(const char* library, const char* name) const noexcept
{
    //iterate over all layers
    uint64_t layerCount = m_layers.byteSize / m_layers.elementSize;
    for (size_t i = 0; i < layerCount; ++i)
    {
        //get the current layer
        const LayerStackElement& el = ((LayerStackElement*)m_layers.ptr)[i];

        //get the layer type
        LayerType type = getLayerType(el);

        //check if the types match
        if (!(strcmp(library, type.library) || strcmp(name, type.name)))
        {
            //if they do, return the index
            return i;
        }
    }

    //no type found, return the error value
    return SIZE_MAX;
}

bool LayerStack::removeLayer(size_t index)
{
    //sanity check -> stop if it is out of bounds
    if ((index * m_layers.elementSize) >= m_layers.byteSize) {return false;}
    //get the element to clean up
    LayerStackElement& el = *(((LayerStackElement*)m_layers.ptr) + index);
    //call the correct destructor
    if (el.isLayerBase)
    {delete (LayerBase*)el.layer;}
    else
    {delete (Layer*)el.layer;}
    el.layer = NULL;
    //just erase the element
    m_layers.erase(index);
    //success
    return true;
}

bool LayerStack::removeLayer(const char* library, const char* name)
{
    //get the index of the layer
    size_t idx = getLayerIndex(library, name);
    //remove the index
    return removeLayer(idx);
}

void LayerStack::sendEvent(const Event& event)
{
    //iterate over all layers
    uint64_t size = m_layers.byteSize / m_layers.elementSize;
    for (uint64_t i = 0; i < size; ++i)
    {
        //get the element
        LayerStackElement& el = *(((LayerStackElement*)m_layers.ptr) + i);
        //switch between the type of layer
        if (el.isLayerBase)
        {
            //only send the event if the function is set
            if (((LayerBase*)el.layer)->getEventHandler().getHandleFunction())
            {((LayerBase*)el.layer)->getEventHandler().sendEvent(event);}
        }
    }
}





LayerStack* layerStack_CreateFromLayers(Layer** layers, uint64_t layerCount) {
    //map the inputted data to an std::vector
    std::vector<Layer*> tmp(layerCount);
    memcpy(tmp.data(), layers, layerCount*sizeof(*layers));
    //return the new layer stack
    return new LayerStack(tmp);
}

LayerStack* layerStack_CreateFromLayerBases(LayerBase** layers, uint64_t layerCount) {
    //map the inputted data to an std::vector
    std::vector<Layer*> tmp(layerCount);
    memcpy(tmp.data(), layers, layerCount*sizeof(*layers));
    //return the new layer stack
    return new LayerStack(tmp);
}

LayerStack* layerStack_CreateFromLayerStackElements(LayerStackElement* layers, uint64_t layerCount) {
    //map the inputted data to an std::vector
    std::vector<Layer*> tmp(layerCount);
    memcpy(tmp.data(), layers, layerCount*sizeof(*layers));
    //return the new layer stack
    return new LayerStack(tmp);
}

LayerStack* layerStack_CreateFromVector(const Vector* layers, bool isLayerBase) {return new LayerStack(*layers, isLayerBase);}

void layerStack_Delete(LayerStack* ls) {delete ls;}

void layerStack_SignalStartup(LayerStack* ls) {ls->signalStartup();}

void layerStack_Update(LayerStack* ls) {ls->update();}

void layerStack_SignalShutdown(LayerStack* ls) {ls->signalShutdown();}

const LayerStackElement* layerStack_GetElement(const char* library, const char* identifier, LayerStack* ls)
{return ls->getElement(library, identifier);}

const LayerStackElement* layerStack_GetElementIndexed(size_t index, LayerStack* ls)
{return ls->getElement(index);}

bool layerStack_AddNewLayer_Layer(Layer* layer, LayerStack* ls)
{return ls->addNewLayer(layer);}

bool layerStack_AddNewLayer_LayerBase(LayerBase* layer, LayerStack* ls)
{return ls->addNewLayer(layer);}

bool layerStack_AddNewLayer_LayerStackElement(LayerStackElement lse, LayerStack* ls)
{return ls->addNewLayer(lse);}

size_t layerStack_GetLayerIndex(const char* library, const char* name, LayerStack* ls)
{return ls->getLayerIndex(library, name);}

bool layerStack_RemoveLayerIndexed(size_t index, LayerStack* ls)
{return ls->removeLayer(index);}

bool layerStack_RemoveLayer(const char* library, const char* name, LayerStack* ls)
{return ls->removeLayer(library, name);}