/**
 * @file FileDecorator.h
 * @author DM8AT
 * @brief define an interface that can manipulate files being saved and loaded
 * @version 0.1
 * @date 2025-09-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_FILE_DECORATOR_
#define _GLGE_CORE_FILE_DECORATOR_

//include types and strings
#include "../Types.h"
#include "../../GLGE_BG/CBinding/String.h"

/**
 * @brief define a wrapper function for a file decorator
 */
typedef struct s_FileDecorator {
    //store a function that is called to pre-process the data for the file
    //this is called when the file is loaded
    void (*preprocessor)(String* contents, void* userData);
    //store a function that is called to post-process the data for saving
    //this is called when the file is saved
    void (*postprocessor)(String* contents, void* userData);
    //a function that is called when the element is added to a file
    void (*onOpen)(String* path, void* userData);
    //a function that is called when the file the element is attached to is closed
    void (*onClose)(String* path, void* userData);
    //store some arbitrary data that is passed to all called functions
    void* userData;
} FileDecorator;

#endif