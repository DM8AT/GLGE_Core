/**
 * @file EventHandler.cpp
 * @author DM8AT
 * @brief implement the event handler interface
 * @version 0.1
 * @date 2025-09-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the event handler
#include "EventHandler.h"

void EventHandler::sendEventDeferred(const Event& event) noexcept
{
    //lock the mutex to take ownership of the event queue
    m_mut.lock();
    //add the new event
    m_queue.push_back(event);
    //finished -> unlock the mutex
    m_mut.unlock();
}

void EventHandler::handleEvents() noexcept
{
    //if no events exist, early out
    if (m_queue.size() == 0) {return;}
    
    //take ownership of the event queue
    m_mut.lock();
    
    //iterate over all events
    for (size_t i = 0; i < m_queue.size(); ++i)
    {
        //handle the event
        sendEventInstant(m_queue[i]);
    }
    //clear the queue - all clean
    m_queue.clear();

    //finished -> free the event queue
    m_mut.unlock();
}