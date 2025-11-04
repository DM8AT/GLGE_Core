/**
 * @file Message.h
 * @author DM8AT
 * @brief define an interface for messages to pass between listeners
 * @version 0.1
 * @date 2025-09-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_MESSAGE_
#define _GLGE_CORE_MESSAGE_

//include the types
#include "../Types.h"
//include strings
#include "../../GLGE_BG/CBinding/String.h"

/**
 * @brief store the contents of a message
 */
typedef struct s_MessageContents {
    union {
        //store a pointer to data somewhere else
        struct {
            //store a pointer to the data to pass
            void* data;
            //store the amount of bytes in the data
            size_t dataSize;
        };
        //store some in-place data
        byte inPlace[16];
    };
} MessageContents;

/**
 * @brief store the contents of a message
 */
typedef union u_MessageIdentifier {
    //store an in-place identification value
    uint64_t number;
    //store a string as identification
    const char* name;
} MessageIdentifier;

//define that the message will exist
struct s_Message;

/**
 * @brief store all supported message callback functions
 */
typedef union u_MessageCallbackFunc {
    //functions with input void
    void  (*func_retVoid_inpVoid) (void);
    bool  (*func_retBool_inpVoid) (void);
    lint  (*func_retLInt_inpVoid) (void);
    luint (*func_retLUInt_inpVoid)(void);
    int   (*func_retInt_inpVoid)  (void);
    uint  (*func_retUInt_inpVoid) (void);
    sint  (*func_retSInt_inpVoid) (void);
    suint (*func_retSUInt_inpVoid)(void);
    char  (*func_retChar_inpVoid) (void);
    uchar (*func_retUChar_inpVoid)(void);

    //functions with the message as input
    void  (*func_retVoid_inpMsg) (struct s_Message*);
    bool  (*func_retBool_inpMsg) (struct s_Message*);
    lint  (*func_retLInt_inpMsg) (struct s_Message*);
    luint (*func_retLUInt_inpMsg)(struct s_Message*);
    int   (*func_retInt_inpMsg)  (struct s_Message*);
    uint  (*func_retUInt_inpMsg) (struct s_Message*);
    sint  (*func_retSInt_inpMsg) (struct s_Message*);
    suint (*func_retSUInt_inpMsg)(struct s_Message*);
    char  (*func_retChar_inpMsg) (struct s_Message*);
    uchar (*func_retUChar_inpMsg)(struct s_Message*);
} MessageCallbackFunc;

/**
 * @brief define a list of enum identifiers for the different types of message callback functions
 */
typedef enum e_MessageCallbackIdentifier {
    GLGE_MSG_CALLBACK_FUNC_RET_VOID_INP_VOID,
    GLGE_MSG_CALLBACK_FUNC_RET_BOOL_INP_VOID,
    GLGE_MSG_CALLBACK_FUNC_RET_LINT_INP_VOID,
    GLGE_MSG_CALLBACK_FUNC_RET_LUINT_INP_VOID,
    GLGE_MSG_CALLBACK_FUNC_RET_INT_INP_VOID,
    GLGE_MSG_CALLBACK_FUNC_RET_UINT_INP_VOID,
    GLGE_MSG_CALLBACK_FUNC_RET_SINT_INP_VOID,
    GLGE_MSG_CALLBACK_FUNC_RET_SUINT_INP_VOID,
    GLGE_MSG_CALLBACK_FUNC_RET_CHAR_INP_VOID,
    GLGE_MSG_CALLBACK_FUNC_RET_UCHAR_INP_VOID,

    GLGE_MSG_CALLBACK_FUNC_RET_VOID_INP_PMSG,
    GLGE_MSG_CALLBACK_FUNC_RET_BOOL_INP_PMSG,
    GLGE_MSG_CALLBACK_FUNC_RET_LINT_INP_PMSG,
    GLGE_MSG_CALLBACK_FUNC_RET_LUINT_INP_PMSG,
    GLGE_MSG_CALLBACK_FUNC_RET_INT_INP_PMSG,
    GLGE_MSG_CALLBACK_FUNC_RET_UINT_INP_PMSG,
    GLGE_MSG_CALLBACK_FUNC_RET_SINT_INP_PMSG,
    GLGE_MSG_CALLBACK_FUNC_RET_SUINT_INP_PMSG,
    GLGE_MSG_CALLBACK_FUNC_RET_CHAR_INP_PMSG,
    GLGE_MSG_CALLBACK_FUNC_RET_UCHAR_INP_PMSG,
} MessageCallbackIdentifier;

/**
 * @brief combines a message callback function with a message callback function identifier
 */
typedef struct s_MessageCallback
{
    //store the message callback identifier
    MessageCallbackIdentifier identifier;
    //store the message callback function
    MessageCallbackFunc func;
} MessageCallback;


/**
 * @brief store a message to send
 */
typedef struct s_Message {
    //store the message identifier
    MessageIdentifier identifier;
    //store the message's contents
    MessageContents contents;
    //store the message callback function
    MessageCallback callback;
} Message;

#endif