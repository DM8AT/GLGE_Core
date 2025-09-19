/**
 * @file MessageListener.h
 * @author DM8AT
 * @brief define an interface for a message listener
 * @version 0.1
 * @date 2025-09-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_MESSAGE_LISTENER_
#define _GLGE_CORE_MESSAGE_LISTENER_

//include the message types
#include "Message.h"

//for C++ add a special class
#if __cplusplus

//include vectors to queue messages
#include <vector>

/**
 * @brief a simple class that can 
 */
class MessageListener
{
public:

    /**
     * @brief create a new message listener base instance
     */
    MessageListener() = default;

    /**
     * @brief send a message to the message listener
     * 
     * @param msg a constant reference to the message to send
     */
    inline void sendMessage(const Message& msg) noexcept {m_queue.push_back(msg);}

    /**
     * @brief get the bottom most message from the stack
     * 
     * @param msg a pointer to fill with the message
     * @return true : a message was pulled
     * @return false : no message was pulled
     */
    bool pull(Message* msg) noexcept;

    /**
     * @brief get a reference to the next message if it exists
     * 
     * @param msg a pointer to a message to fill with the next message or do nothing with if it does not exist
     * @return true : a next message was found
     * @return false : no next message was found, the current message is the last one
     */
    bool peek(Message* msg) noexcept;

    /**
     * @brief dump all data currently resident in the queue
     */
    void clear() noexcept {m_queue.clear();}

    /**
     * @brief Get how many messages are still queued up
     * 
     * @return size_t the amount of queued messages
     */
    size_t getMessageCount() const noexcept {return m_queue.size();}

    /**
     * @brief Get the Message at a specific index
     * 
     * @warning the index is not checked
     * 
     * @param i the index of the message to quarry
     * @return const Message& a constant reference to the message
     */
    inline const Message& getMessage(size_t i) const noexcept {return m_queue[i];}

    /**
     * @brief Get the Message at a specific index
     * 
     * @warning the index is not checked
     * 
     * @param i the index of the message to quarry
     * @return const Message& a constant reference to the message
     */
    inline const Message& operator[](size_t i) const noexcept {return getMessage(i);}

protected:

    /**
     * @brief store a list of all messages
     */
    std::vector<Message> m_queue;

};

//for C use a dummy struct
#else

/**
 * @brief store an opaque structure for a message listener
 */
typedef struct s_MessageListener {byte dummy;} MessageListener;

#endif

/**
 * @brief create a new message listener instance
 * 
 * @return MessageListener* a pointer to the new message listener
 */
MessageListener* messageListener_Create();

/**
 * @brief delete a message listener instance
 * 
 * @param ml a pointer to the message listener instance to delete
 */
void messageListener_Delete(MessageListener* ml);

/**
 * @brief send a message to a message listener instance
 * 
 * @param msg a constant pointer to the message to send
 * @param ml a pointer to the message listener instance to send to
 */
void messageListener_SendMessage(const Message* msg, MessageListener* ml);

/**
 * @brief pull a message from a message listener
 * 
 * @param msg a pointer to a message to store the pulled message from
 * @param ml a pointer to the message listener to pull the message from
 * @return true : a message was pulled successfully
 * @return false : no message was pulled
 */
bool messageListener_Pull(Message* msg, MessageListener* ml);

/**
 * @brief peek forward in the message stack
 * 
 * @param msg a pointer to a message to store the message to
 * @param ml a pointer to the message stack instance to peek into
 * @return true : a message was peeked at
 * @return false : no message for peeking found
 */
bool messageListener_Peek(Message* msg, MessageListener* ml);

/**
 * @brief remove all messages from a message stack
 * 
 * @param ml a pointer to the message stack to remove all messages from
 */
void messageListener_Clear(MessageListener* ml);

/**
 * @brief get the amount of messages currently queued
 * 
 * @param ml a pointer to the message listener instance to pull the amount of messages from
 * @return size_t the amount of currently queued messages
 */
size_t messageListener_GetMessageCount(MessageListener* ml);

/**
 * @brief get a message from a specific index from the message stack
 * 
 * @param i the integer index into the message queue to pull from
 * @param ml a pointer to the message listener to pull from
 * @return const Message* a pointer to the pulled message
 */
const Message* messageListener_GetMessage(size_t i, MessageListener* ml);

#endif