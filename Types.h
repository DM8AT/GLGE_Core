/**
 * @file Types.h
 * @author DM8AT
 * @brief define some simple utility types
 * @version 0.1
 * @date 2025-09-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_TYPES_
#define _GLGE_CORE_TYPES_

//include the C default types
#include <stdint.h>
#include <stdlib.h>

//define a type for a single byte
typedef uint8_t byte;

//define atype wrapper for an unsigned int
typedef unsigned int uint;

//define a type wrapper for a long int
typedef long int lint;

//define a type wrapper for a long unsigned int
typedef long unsigned int luint;

//define a type for a short int
typedef short int sint;

//define a type for a short unsigned int
typedef short unsigned int suint;

//define a type for an unsigned char
typedef unsigned char uchar;

//define a type for a half float
//check for clang or gcc
#if defined(__clang__) || defined(__GNUC__)
    //just us the pre-define float 16
    typedef _Float16 half;
//else, check for MSVC
#elif defined(_MSC_VER)
    //use the intrinsic float 16 stuff
    #include <intrin.h>
    #if defined(__AVX512FP16__)
        typedef __fp16 half;
    #else
        //if the intrinsic mode is not supported, this is a warning (imitate a half using uint16_t's)
        #if defined(_MSC_VER)
            #if _MSC_VER < 1934
                #pragma message("MSVC version too old for AVX512FP16 (requires 19.34 or newer)")
            #endif
        #else
            #warning "Non-MSVC compiler: make sure it supports AVX512FP16"
        #endif
        typedef uint16_t half;
    #endif
#else
    //unknown compiler -> warning + imitate half using uint16_t's
    #pragma message ("Unsupported compiler for half-precision float")
    typedef uint16_t half;
#endif

//a simple utility define used for quick, cross-platform access to NAN
#define GLGE_NAN_F ((union { uint32_t i; float f; }){0x7fffffffu}.f)

#endif