/**
 * @file VertexLayout.cpp
 * @author DM8AT
 * @brief implement the C mapping for a vertex layout
 * @version 0.1
 * @date 2025-10-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include vertex layouts
#include "VertexLayout.h"

void vertexLayout_Create(VertexElement* elements, uint64_t elementCount, VertexLayout* layout)
{*layout = VertexLayout(elements, elementCount);}

uint64_t vertexLayout_GetVertexSize(VertexLayout* layout)
{return layout->getVertexSize();}

uint64_t vertexLayout_GetIndexOfElement(VertexElementType type, VertexLayout* layout)
{return layout->getIndexOfElement(type);}