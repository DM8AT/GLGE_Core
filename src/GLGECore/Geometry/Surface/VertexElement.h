/**
 * @file VertexElement.h
 * @author DM8AT
 * @brief define what a single element of a vertex is
 * @version 0.1
 * @date 2025-10-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_GRAPHIC_SURFACE_VERTEX_ELEMENT_
#define _GLGE_CORE_GRAPHIC_SURFACE_VERTEX_ELEMENT_

//include the types
#include "../../Types.h"
//include the vector types
#include "../../../GLGE_Math/GLGEMath.h"

/**
 * @brief define some types (meanings) a vertex element may have
 */
typedef enum e_VertexElementType {
    //the meaning of the vertex element is custom / undefined / uninitialized
    VERTEX_ELEMENT_TYPE_UNDEFINED = 0,
    //the element is a position
    VERTEX_ELEMENT_TYPE_POSITION,
    //the element represents a normal vector
    VERTEX_ELEMENT_TYPE_NORMAL,
    //the element represents a tangent
    VERTEX_ELEMENT_TYPE_TANGENT,
    //the element represents a bitangent
    VERTEX_ELEMENT_TYPE_BITANGENT,

    //the element represents a texture coordinate
    VERTEX_ELEMENT_TYPE_TEXTURE_COORDINATE0,
    //the element represents a texture coordinate
    VERTEX_ELEMENT_TYPE_TEXTURE_COORDINATE1,
    //the element represents a texture coordinate
    VERTEX_ELEMENT_TYPE_TEXTURE_COORDINATE2,
    //the element represents a texture coordinate
    VERTEX_ELEMENT_TYPE_TEXTURE_COORDINATE3,
    //the element represents a texture coordinate
    VERTEX_ELEMENT_TYPE_TEXTURE_COORDINATE4,
    //the element represents a texture coordinate
    VERTEX_ELEMENT_TYPE_TEXTURE_COORDINATE5,
    //the element represents a texture coordinate
    VERTEX_ELEMENT_TYPE_TEXTURE_COORDINATE6,
    //the element represents a texture coordinate
    VERTEX_ELEMENT_TYPE_TEXTURE_COORDINATE7,

    //the element represents a color
    VERTEX_ELEMENT_TYPE_COLOR0,
    //the element represents a color
    VERTEX_ELEMENT_TYPE_COLOR1,
    //the element represents a color
    VERTEX_ELEMENT_TYPE_COLOR2,
    //the element represents a color
    VERTEX_ELEMENT_TYPE_COLOR3,
    //the element represents a color
    VERTEX_ELEMENT_TYPE_COLOR4,
    //the element represents a color
    VERTEX_ELEMENT_TYPE_COLOR5,
    //the element represents a color
    VERTEX_ELEMENT_TYPE_COLOR6,
    //the element represents a color
    VERTEX_ELEMENT_TYPE_COLOR7,
} VertexElementType;

//define how many different types of vertex element types exist
#define VERTEX_ELEMENT_TYPE_COUNT 21

/**
 * @brief define an identifier for a single type of vertex element
 */
typedef enum e_VertexElementDataType {
    //undefined / default data type
    VERTEX_ELEMENT_DATA_TYPE_UNDEFINED = 0,

    //integer types

    VERTEX_ELEMENT_DATA_TYPE_INT8,
    VERTEX_ELEMENT_DATA_TYPE_UINT8,
    VERTEX_ELEMENT_DATA_TYPE_INT16,
    VERTEX_ELEMENT_DATA_TYPE_UINT16,
    VERTEX_ELEMENT_DATA_TYPE_INT32,
    VERTEX_ELEMENT_DATA_TYPE_UINT32,
    VERTEX_ELEMENT_DATA_TYPE_INT64,
    VERTEX_ELEMENT_DATA_TYPE_UINT64,

    //float types

    VERTEX_ELEMENT_DATA_TYPE_HALF,
    VERTEX_ELEMENT_DATA_TYPE_FLOAT,
    VERTEX_ELEMENT_DATA_TYPE_DOUBLE,

    //half vector types

    VERTEX_ELEMENT_DATA_TYPE_HALF_VEC2,
    VERTEX_ELEMENT_DATA_TYPE_HALF_VEC3,
    VERTEX_ELEMENT_DATA_TYPE_HALF_VEC4,

    //float vector types

    VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC2,
    VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC3,
    VERTEX_ELEMENT_DATA_TYPE_FLOAT_VEC4,

    //double vector types

    VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC2,
    VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC3,
    VERTEX_ELEMENT_DATA_TYPE_DOUBLE_VEC4,

    //int8 vector types

    VERTEX_ELEMENT_DATA_TYPE_INT8_VEC2,
    VERTEX_ELEMENT_DATA_TYPE_INT8_VEC3,
    VERTEX_ELEMENT_DATA_TYPE_INT8_VEC4,

    //uint8 vector types

    VERTEX_ELEMENT_DATA_TYPE_UINT8_VEC2,
    VERTEX_ELEMENT_DATA_TYPE_UINT8_VEC3,
    VERTEX_ELEMENT_DATA_TYPE_UINT8_VEC4,

    //int16 vector types

    VERTEX_ELEMENT_DATA_TYPE_INT16_VEC2,
    VERTEX_ELEMENT_DATA_TYPE_INT16_VEC3,
    VERTEX_ELEMENT_DATA_TYPE_INT16_VEC4,

    //uint16 vector types

    VERTEX_ELEMENT_DATA_TYPE_UINT16_VEC2,
    VERTEX_ELEMENT_DATA_TYPE_UINT16_VEC3,
    VERTEX_ELEMENT_DATA_TYPE_UINT16_VEC4,

    //int32 vector types

    VERTEX_ELEMENT_DATA_TYPE_INT32_VEC2,
    VERTEX_ELEMENT_DATA_TYPE_INT32_VEC3,
    VERTEX_ELEMENT_DATA_TYPE_INT32_VEC4,

    //uint32 vector types

    VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC2,
    VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC3,
    VERTEX_ELEMENT_DATA_TYPE_UINT32_VEC4,

    //int64 vector types

    VERTEX_ELEMENT_DATA_TYPE_INT64_VEC2,
    VERTEX_ELEMENT_DATA_TYPE_INT64_VEC3,
    VERTEX_ELEMENT_DATA_TYPE_INT64_VEC4,

    //uint64 vector types

    VERTEX_ELEMENT_DATA_TYPE_UINT64_VEC2,
    VERTEX_ELEMENT_DATA_TYPE_UINT64_VEC3,
    VERTEX_ELEMENT_DATA_TYPE_UINT64_VEC4,

} VertexElementDataType;

/**
 * @brief define a vertex element as a paring of a vertex element type and a data type
 */
typedef struct s_VertexElement {
    //the type of the vertex element
    VertexElementType type;
    //the data type of the element
    VertexElementDataType data;

    //for C++ create a constructor
    #if __cplusplus
    
    /**
     * @brief create a new vertex element
     */
    constexpr s_VertexElement()
     : type(VERTEX_ELEMENT_TYPE_UNDEFINED),
       data(VERTEX_ELEMENT_DATA_TYPE_UNDEFINED)
    {}

    /**
     * @brief create a new vertex element
     * 
     * @param _type the type of vertex element
     * @param _data the type of the data belonging to the vertex element
     */
    constexpr s_VertexElement(const VertexElementType& _type, const VertexElementDataType& _data)
     : type(_type), data(_data)
    {}

    /**
     * @brief check if two vertex elements are the same
     * 
     * @param other the vertex element to check agains
     * @return true : they are the same
     * @return false : they are not the same
     */
    inline constexpr bool operator==(const s_VertexElement& other) const noexcept 
    {return (type == other.type) && (data == other.data);}

    #endif

} VertexElement;

#endif