/**
 * @file MessageListener.cpp
 * @author DM8AT
 * @brief define the functions for a message listener as well as the C binding
 * @version 0.1
 * @date 2025-09-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the message listener API
#include "MessageListener.h"

bool MessageListener::pull(Message* msg) noexcept
{
    //if the size is 0, stop
    if (m_queue.size() == 0) {return false;}

    //else, store the bottom most message. But only if msg is not NULL
    if (msg) {*msg = m_queue[0];}
    //and delete the first element
    m_queue.erase(m_queue.begin());
    //but the element was found
    return true;
}

bool MessageListener::peek(Message* msg) noexcept
{
    //if the size is 0 or 1, stop
    if (m_queue.size() < 2) {return false;}

    //else, store the next message, but only if msg is not NULL
    if (msg) {*msg = m_queue[1];}
    //but the element was found
    return true;
}

bool MessageListener::callMessageCallback(Message& msg, void* ret) noexcept
{
    //if no callback exists, stop
    if (!msg.callback.func.func_retBool_inpMsg) {return false;}

    //sanity check the pointer
    if ((msg.callback.identifier != GLGE_MSG_CALLBACK_FUNC_RET_VOID_INP_VOID) && (msg.callback.identifier != GLGE_MSG_CALLBACK_FUNC_RET_VOID_INP_PMSG)
        && (!ret))
    {return false;}

    //else, switch over the type of callback function
    switch (msg.callback.identifier)
    {
    //input void callback
    case GLGE_MSG_CALLBACK_FUNC_RET_VOID_INP_VOID:
        (*msg.callback.func.func_retVoid_inpVoid)();
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_BOOL_INP_VOID:
        *((bool*)ret) = (*msg.callback.func.func_retBool_inpVoid)();
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_LINT_INP_VOID:
        *((lint*)ret) = (*msg.callback.func.func_retLInt_inpVoid)();
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_LUINT_INP_VOID:
        *((luint*)ret) = (*msg.callback.func.func_retLUInt_inpVoid)();
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_INT_INP_VOID:
        *((int*)ret) = (*msg.callback.func.func_retInt_inpVoid)();
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_UINT_INP_VOID:
        *((uint*)ret) = (*msg.callback.func.func_retUInt_inpVoid)();
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_SINT_INP_VOID:
        *((sint*)ret) = (*msg.callback.func.func_retSInt_inpVoid)();
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_SUINT_INP_VOID:
        *((suint*)ret) = (*msg.callback.func.func_retSUInt_inpVoid)();
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_CHAR_INP_VOID:
        *((char*)ret) = (*msg.callback.func.func_retChar_inpVoid)();
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_UCHAR_INP_VOID:
        *((uchar*)ret) = (*msg.callback.func.func_retUChar_inpVoid)();
        break;

    //input message callback
    case GLGE_MSG_CALLBACK_FUNC_RET_VOID_INP_PMSG:
        (*msg.callback.func.func_retVoid_inpMsg)(&msg);
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_BOOL_INP_PMSG:
        *((bool*)ret) = (*msg.callback.func.func_retBool_inpMsg)(&msg);
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_LINT_INP_PMSG:
        *((lint*)ret) = (*msg.callback.func.func_retLInt_inpMsg)(&msg);
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_LUINT_INP_PMSG:
        *((luint*)ret) = (*msg.callback.func.func_retLUInt_inpMsg)(&msg);
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_INT_INP_PMSG:
        *((int*)ret) = (*msg.callback.func.func_retInt_inpMsg)(&msg);
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_UINT_INP_PMSG:
        *((uint*)ret) = (*msg.callback.func.func_retUInt_inpMsg)(&msg);
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_SINT_INP_PMSG:
        *((sint*)ret) = (*msg.callback.func.func_retSInt_inpMsg)(&msg);
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_SUINT_INP_PMSG:
        *((suint*)ret) = (*msg.callback.func.func_retSUInt_inpMsg)(&msg);
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_CHAR_INP_PMSG:
        *((char*)ret) = (*msg.callback.func.func_retChar_inpMsg)(&msg);
        break;
    case GLGE_MSG_CALLBACK_FUNC_RET_UCHAR_INP_PMSG:
        *((uchar*)ret) = (*msg.callback.func.func_retUChar_inpMsg)(&msg);
        break;

    default:
        return false;
        break;
    }
    //success
    return true;
}

MessageListener* messageListener_Create() {return new MessageListener();}

void messageListener_Delete(MessageListener* ml) {delete ml;}

void messageListener_SendMessage(const Message* msg, MessageListener* ml) {ml->sendMessage(*msg);}

bool messageListener_Pull(Message* msg, MessageListener* ml) {return ml->pull(msg);}

bool messageListener_Peek(Message* msg, MessageListener* ml) {return ml->peek(msg);}

void messageListener_Clear(MessageListener* ml) {ml->clear();}

size_t messageListener_GetMessageCount(MessageListener* ml) {return ml->getMessageCount();}

const Message* messageListener_GetMessage(size_t i, MessageListener* ml) {return &(ml->getMessage(i));}
