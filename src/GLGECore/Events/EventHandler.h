/**
 * @file EventHandler.h
 * @author DM8AT
 * @brief define an interface for a structure that can handle events
 * @version 0.1
 * @date 2025-09-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_EVENT_LISTENER_
#define _GLGE_CORE_EVENT_LISTENER_

//include the definition of events
#include "Event.h"

//for C++ create a class
#if __cplusplus

//include std::vectors for simple queueing
#include <vector>
//include a mutex to make the events thread-safe
#include <mutex>
//include error handling
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

/**
 * @brief a class to handle events
 */
class EventHandler
{
public:

    /**
     * @brief Construct a new Event Handler
     * 
     * @param func a callback function for the event handler
     * @param userData some arbitrary user data to pass to the function
     * @param deferEvents say if events should be evaluated directly or if they should be queued up
     */
    EventHandler(void (*func)(const Event*, void*), void* userData, bool deferEvents = false) noexcept
     : m_eventHandleFunc(func), m_deferEvents(deferEvents) , m_userData(userData)
    {GLGE_ASSERT("Can't create a message handler with a callback function of NULL", !func);}

    /**
     * @brief send an event to the envent handler
     * 
     * @param event a reference to the event to handle
     */
    void sendEvent(const Event& event)noexcept {
        //handle the event according to the requested deferring mode
        if (m_deferEvents) {sendEventDeferred(event);} 
        else {sendEventInstant(event);}
    }

    /**
     * @brief queue an event and force it to be handled in a deferred manner
     * 
     * @param event a constant reference to the event to queue
     */
    void sendEventDeferred(const Event& event) noexcept;

    /**
     * @brief send an event to the event handler that requires instant handling
     * 
     * @param event a constant reference to the event to handle
     */
    void sendEventInstant(const Event& event) noexcept {(*m_eventHandleFunc)(&event, m_userData);}

    /**
     * @brief handle all currently queued events
     */
    void handleEvents() noexcept;

    /**
     * @brief Set if events should happen deferred or instantly by default
     * 
     * @param deferEvents true : events will be first queued | false : events will be handled instantly
     */
    inline void setDeferEvents(bool deferEvents) {m_deferEvents = deferEvents;}

    /**
     * @brief Get if events are handled instant or deferred by default
     * 
     * @return true : events are handled in a deferred manner by default | 
     * @return false : events are handled instantly by default
     */
    inline bool getDeferEvents() const noexcept {return m_deferEvents;}

protected:

    /**
     * @brief store a function to handle the events
     */
    void (*m_eventHandleFunc)(const Event*,void*);

    /**
     * @brief store if events will execute immediately or if events will be queued up and run at a later time point
     */
    bool m_deferEvents = false;

    /**
     * @brief store some arbitrary user data
     */
    void* m_userData = NULL;

    /**
     * @brief store a queue of all events that occurred
     */
    std::vector<Event> m_queue;
    /**
     * @brief store a mutex to sync the event queueing between threads
     */
    std::mutex m_mut;

};

#else //for C add an opaque structure

/**
 * @brief store an event handler
 * 
 * @warning do only use as a pointer, NEVER de-reference the structure
 */
typedef struct s_EventHandler {byte unused;} EventHandler;

#endif

/**
 * @brief create a new event handler
 * 
 * @param func the function to handle the events with
 * @param userData some arbitrary user data
 * @param deferEvents true : the events will first be queued, then handled | false : the events will be handled instantly
 * @return EventHandler* a pointer to the new event handler
 */
EventHandler* eventHandler_Create(void (*func)(const Event*, void*), void* userData, bool deferEvents);

/**
 * @brief delete an event handler
 * 
 * @param eh a pointer to the event handler to delete
 */
void eventHandler_Delete(EventHandler* eh);

/**
 * @brief send an event to an event handler
 * 
 * @param event the event to send to the event handler
 * @param eh a pointer to the event handler to send the message to
 */
void eventHandler_sendEvent(Event event, EventHandler* eh);

/**
 * @brief queue an event and force it to be handled in a deferred manner
 * 
 * @param event the event to queue
 * @param eh a pointer to an event handler to send the event to
 */
void eventHandler_SendEventDeferred(Event event, EventHandler* eh);

/**
 * @brief send an event to the event handler that requires instant handling
 * 
 * @param event the event to handle
 * @param eh a pointer to an event handler to send the event to
 */
void eventHandler_sendEventInstant(Event event, EventHandler* eh);

/**
 * @brief handle all currently queued events
 * 
 * @param eh a pointer to the event handler to handle all events from
 */
void eventHandler_handleEvents(EventHandler* eh);

/**
 * @brief Set if events should happen deferred or instantly by default
 * 
 * @param deferEvents true : events will be first queued | false : events will be handled instantly
 * @param eh a pointer to the event handler to set if it should defer events
 */
void eventHandler_setDeferEvents(bool deferEvents, EventHandler* eh);

/**
 * @brief Get if events are handled instant or deferred by default
 * 
 * @param eh a pointer to the event handler to quarry the data from
 * @return true : events are handled in a deferred manner by default | 
 * @return false : events are handled instantly by default
 */
bool eventHandler_getDeferEvents(EventHandler* eh);

#endif