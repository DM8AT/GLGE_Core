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

MessageListener* messageListener_Create() {return new MessageListener();}

void messageListener_Delete(MessageListener* ml) {delete ml;}

void messageListener_SendMessage(const Message* msg, MessageListener* ml) {ml->sendMessage(*msg);}

bool messageListener_Pull(Message* msg, MessageListener* ml) {return ml->pull(msg);}

bool messageListener_Peek(Message* msg, MessageListener* ml) {return ml->peek(msg);}

void messageListener_Clear(MessageListener* ml) {ml->clear();}

size_t messageListener_GetMessageCount(MessageListener* ml) {return ml->getMessageCount();}

const Message* messageListener_GetMessage(size_t i, MessageListener* ml) {return &(ml->getMessage(i));}
