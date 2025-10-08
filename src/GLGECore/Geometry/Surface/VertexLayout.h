/**
 * @file s_VertexLayout.h
 * @author DM8AT
 * @brief define what a vertex layout is
 * @version 0.1
 * @date 2025-10-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_GRAPHIC_SURFACE_VERTEX_LAYOUT_
#define _GLGE_CORE_GRAPHIC_SURFACE_VERTEX_LAYOUT_

//include what an element of a vertex is
#include "VertexElement.h"
//add strings
#include "../../../GLGE_BG/CBinding/String.h"

//check if this is C++
#if __cplusplus
    //include map for a constructor
    #include <map>
    //include arrays for ordered storage
    #include <array>
#endif

/**
 * @brief store an actual vertex
 */
typedef struct s_VertexLayout {

    //member functions for C++
    #if __cplusplus 

    /**
     * @brief Construct a new Vertex Layout
     * 
     * @param elements a map of element types to the element data types
     */
    s_VertexLayout(const std::map<VertexElementType, VertexElementDataType>& elements) noexcept
    {
        //store the index of the element
        uint64_t index = 0;
        //iterate over all elements and store them correctly
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            //store the element in the element map at the correct index
            m_elements[index] = VertexElement(it->first, it->second);
        }
        //calculate the cache size
        __cache_vertex_size();
    }

    /**
     * @brief Construct a new Vertex Layout
     * 
     * @param initList an initializer list containing all the elements for the vertex layout
     */
    constexpr s_VertexLayout(const std::initializer_list<VertexElement>& initList) noexcept
    {
        //iterate over all elements in the initializer list
        for (uint64_t i = 0; i < initList.size(); ++i) {
            //get the element
            VertexElement el = VertexElement((initList.begin() + i)->type, (initList.begin() + i)->data);
            //iterate over all allready set up elements and check if they have the current type
            for (uint64_t j = 0; j < i; ++j) {
                if (m_elements[j].type == el.type) {
                    //error -> skip
                    m_invalidConstruction = true;
                    continue;
                }
            }
            //copy the elements over
            m_elements[i] = el;
        }
        //calculate the cache size
        __cache_vertex_size();
    }

    /**
     * @brief Construct a new Vertex Layout
     * 
     * @param elements a C array of elements to initialize the vertex layout from
     * @param elementCount the amount of elements in the C array
     */
    constexpr s_VertexLayout(const VertexElement* elements, uint64_t elementCount) noexcept
    {
        //iterate over all elements and map them correctly
        for (uint64_t i = 0; i < elementCount; ++i) {
            //iterate over all allready set up elements and check if they have the current type
            for (uint64_t j = 0; j < i; ++j) {
                if (m_elements[j].type == elements[i].type) {
                    //error -> skip
                    m_invalidConstruction = true;
                    continue;
                }
            }
            //copy the element over
            m_elements[i] = elements[i];
        }
        //calculate the cache size
        __cache_vertex_size();
    }

    /**
     * @brief Get the size in bytes of a vertex that follows this vertex layout
     * 
     * @return constexpr size_t the size of the vertex in bytes
     */
    inline constexpr size_t getVertexSize() const noexcept {return m_size;}

    /**
     * @brief Get the Index Of a specific element type
     * 
     * @param type the type of the element to quarry
     * @return constexpr uint64_t the index of the element or UINT64_MAX if it was not found
     */
    inline constexpr uint64_t getIndexOfElement(const VertexElementType& type) const noexcept {
        //iterate over all elements and check the type
        for (uint64_t i = 0; i < VERTEX_ELEMENT_TYPE_COUNT; ++i)
        {if (m_elements[i].type == type) {return i;}}
        //if the element type was not found, return undefined
        return UINT64_MAX;
    }

    #endif

    //store a mapping from an index to a vertex element
    std::array<VertexElement, VERTEX_ELEMENT_TYPE_COUNT> m_elements;

    //store if something went wrong with the construction (example: multiple definitions for the same type)
    bool m_invalidConstruction = false;
    //store the size of a whole vertex
    size_t m_size = 0;

    //protected functions for C++
    #if __cplusplus
    protected:

    //calculate and cache the size
    inline constexpr void __cache_vertex_size() noexcept {
        //iterate over all elements and sum up the element size of each one
        size_t sum = 0;
        for (uint64_t i = 0; i < VERTEX_ELEMENT_TYPE_COUNT; ++i) {
            sum += size(m_elements[i].data);
        }
        //store the size in the size cache
        m_size = sum;
    }

    //get the size of the element
    inline static constexpr size_t size(const VertexElementDataType& data) noexcept 
    {
        switch (data) {
            // Integer scalar types
            case VERTEX_ELEMENT_DATA_TYPE_INT8:
            case VERTEX_ELEMENT_DATA_TYPE_UINT8:
                return 1;

            case VERTEX_ELEMENT_DATA_TYPE_INT16:
            case VERTEX_ELEMENT_DATA_TYPE_UINT16:
                return 2;

            case VERTEX_ELEMENT_DATA_TYPE_INT32:
            case VERTEX_ELEMENT_DATA_TYPE_UINT32:
                return 4;

            case VERTEX_ELEMENT_DATA_TYPE_INT64:
            case VERTEX_ELEMENT_DATA_TYPE_UINT64:
                return 8;

            // Floating-point scalar types
            case VERTEX_ELEMENT_DATA_TYPE_HALF:
                return 2;

            case VERTEX_ELEMENT_DATA_TYPE_FLOAT:
                return 4;

            case VERTEX_ELEMENT_DATA_TYPE_DOUBLE:
                return 8;

            // Half vector types
            case VERTEX_ELEMENT_DATA_TYPE_HALF_VEC2:
                return 2 * 2;
            case VERTEX_ELEMENT_DATA_TYPE_HALF_VEC3:
                return 2 * 3;
            case VERTEX_ELEMENT_DATA_TYPE_HALF_VEC4:
                return 2 * 4;

            // Float vector types
            case VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC2:
                return 4 * 2;
            case VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC3:
                return 4 * 3;
            case VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC4:
                return 4 * 4;

            // Double vector types
            case VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC2:
                return 8 * 2;
            case VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC3:
                return 8 * 3;
            case VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC4:
                return 8 * 4;

            // Int8 vector types
            case VERTEX_ELEMENT_DATA_TYPE_INT8_VEC2:
            case VERTEX_ELEMENT_DATA_TYPE_UINT8_VEC2:
                return 1 * 2;
            case VERTEX_ELEMENT_DATA_TYPE_INT8_VEC3:
            case VERTEX_ELEMENT_DATA_TYPE_UINT8_VEC3:
                return 1 * 3;
            case VERTEX_ELEMENT_DATA_TYPE_INT8_VEC4:
            case VERTEX_ELEMENT_DATA_TYPE_UINT8_VEC4:
                return 1 * 4;

            // Int16 vector types
            case VERTEX_ELEMENT_DATA_TYPE_INT16_VEC2:
            case VERTEX_ELEMENT_DATA_TYPE_UINT16_VEC2:
                return 2 * 2;
            case VERTEX_ELEMENT_DATA_TYPE_INT16_VEC3:
            case VERTEX_ELEMENT_DATA_TYPE_UINT16_VEC3:
                return 2 * 3;
            case VERTEX_ELEMENT_DATA_TYPE_INT16_VEC4:
            case VERTEX_ELEMENT_DATA_TYPE_UINT16_VEC4:
                return 2 * 4;

            // Int32 vector types
            case VERTEX_ELEMENT_DATA_TYPE_INT32_VEC2:
            case VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC2:
                return 4 * 2;
            case VERTEX_ELEMENT_DATA_TYPE_INT32_VEC3:
            case VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC3:
                return 4 * 3;
            case VERTEX_ELEMENT_DATA_TYPE_INT32_VEC4:
            case VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC4:
                return 4 * 4;

            // Int64 vector types
            case VERTEX_ELEMENT_DATA_TYPE_INT64_VEC2:
            case VERTEX_ELEMENT_DATA_TYPE_UINT64_VEC2:
                return 8 * 2;
            case VERTEX_ELEMENT_DATA_TYPE_INT64_VEC3:
            case VERTEX_ELEMENT_DATA_TYPE_UINT64_VEC3:
                return 8 * 3;
            case VERTEX_ELEMENT_DATA_TYPE_INT64_VEC4:
            case VERTEX_ELEMENT_DATA_TYPE_UINT64_VEC4:
                return 8 * 4;

            default:
                //Unknown or unsupported type
                return 0;
        }
    }
    #endif

} VertexLayout;

/**
 * @brief create a new vertex layout
 * 
 * @param elements a pointer to an array of elements to create the layout from
 * @param elementCount the amount of elements in the array
 * @param layout a pointer to some form of storage for the layout (must have the size of sizeof(VertexLayout))
 */
void vertexLayout_Create(VertexElement* elements, uint64_t elementCount, VertexLayout* layout);

/**
 * @brief get the size of a vertex that uses the inputted layout in bytes
 * 
 * @param layout a pointer to the layout element to quarry the information from
 * @return uint64_t the size of a vertex in bytes
 */
uint64_t vertexLayout_GetVertexSize(VertexLayout* layout);

/**
 * @brief get the index of a specific element type
 * 
 * @param type the type to quarry the index for
 * @param layout a pointer to the layout to quarry the layout from
 * @return uint64_t the index of the layout with that type or UINT64_MAX if it was not found
 */
uint64_t vertexLayout_GetIndexOfElement(VertexElementType type, VertexLayout* layout);

#endif