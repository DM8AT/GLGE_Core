/**
 * @file GLGE_Message.hpp
 * @author DM8AT
 * @brief define an interface for C to create messages
 * @version 0.1
 * @date 2025-09-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//heder guard
#ifndef _GLGE_CORE_C_MESSAGES_MESSAGE_
#define _GLGE_CORE_C_MESSAGES_MESSAGE_

//in C++ create a C section
#if __cplusplus
extern "C" {
#endif

//specific sized integers are needed
#include <stdint.h>

/**
 * @brief store information about a single message
 */
typedef struct s_MessageType {
    //store the library the message originates from
    uint16_t library;
    //store what kind of message this is
    uint16_t messageID;
} MessageType;

/**
 * @brief store all the information about the message
 */
typedef struct s_Message {
    //store the type of message
    MessageType type;
    //store an optional message callback to call
    void (*callback)();

    //switch between in-place data storage and external data storage
    union {
        uint8_t in_place[16];
        
        struct {
            //store some user defined data
            void* data;
            //store the size of the user defined data
            uint64_t dataSize;
        };
    };
} Message;

//in C++ end a potential C section
#if __cplusplus
}
#endif

#endif