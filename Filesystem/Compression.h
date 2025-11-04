/**
 * @file Compression.h
 * @author DM8AT
 * @brief define some simple functions that can be used with file decorators to implement compression
 * @version 0.1
 * @date 2025-10-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_FILESYSTEM_COMPRESSION_
#define _GLGE_CORE_FILESYSTEM_COMPRESSION_

//include types
#include "../Types.h"
//include strings
#include "../../GLGE_BG/GLGEBg.h"

#if __cplusplus
extern "C" {
#endif

/**
 * @brief compresses a string using the zip compression algorithm
 * 
 * @warning if something goes wrong, the input will be set to an empty string
 * 
 * @param input a string of binary or text data to compress. This is also used as the output. 
 * @param level a pointer to an integer that contains the level of compression to use
 */
void glge_Compress(String* input, void* level);

/**
 * @brief de-compress a string that was compressed using the compress function or using a zip compression algorithm
 * 
 * @warning if something goes wrong, the input will be set to an empty string
 * 
 * @param input a pointer to the input string to de-compress. This is also used as the output. 
 */
void glge_Decompress(String* input, void*);

#if __cplusplus
}
#endif

#endif