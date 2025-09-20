/**
 * @file Event.h
 * @author DM8AT
 * @brief define a structure to store events in
 * @version 0.1
 * @date 2025-09-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_EVENT_
#define _GLGE_CORE_EVENT_

//include all types
#include "../Types.h"
//include C strings for memcpy
#include <string.h>

/**
 * @brief store the type of an event
 */
typedef struct s_EventType {
    //store a name for the library the event is from
    const char* library;
    //store an identifier for the event
    uint64_t event;

    //for C++ add constructors
    #if __cplusplus

    /**
     * @brief Construct a new Event Type
     * 
     * @param _library a string naming the library the event came from
     * @param _event the identifier of the event
     */
    s_EventType(const char* _library, uint64_t _event)
     : library(_library), event(_event)
    {}

    #endif

} EventType;

/**
 * @brief store the data for an event
 */
typedef union u_EventData {
    //store some external data
    struct {
        //store some external data
        void* data;
        //store the size of the external data in bytes
        uint64_t dataSize;
    };
    //store a little bit of data in place
    byte inPlace[16];

    //for C++ create a constructor
    #if __cplusplus

    /**
     * @brief Construct a new Event Data
     * 
     * @param _data a pointer to the data
     * @param _dataSize the size of the data in bytes
     */
    u_EventData(void* _data, uint64_t _dataSize)
     : data(_data), dataSize(_dataSize)
    {}

    /**
     * @brief Construct a new u EventData object
     * 
     * @warning size is not sanity checked. It must be less or equal to 16. 
     * 
     * @param size the size of the in-place data to create
     * @param _inPlace the data to store in place
     */
    u_EventData(uint8_t size, byte* _inPlace)
    {memcpy(inPlace, _inPlace, size);}

    #endif

} EventData;

/**
 * @brief store a single event
 */
typedef struct s_Event {
    //store the type of event
    EventType type;
    //store some data for the event
    EventData data;

    //create constructors for C++
    #if __cplusplus

    /**
     * @brief Construct a new simple event
     * 
     * @param library the library the event came from
     * @param event the type of event
     */
    s_Event(const char* library, uint64_t event)
     : type(library, event), data(nullptr, 0)
    {}

    /**
     * @brief Construct a new Event
     * 
     * @param library the library the event came form
     * @param event the type of event
     * @param _data a pointer to the data to store in the event
     * @param dataSize the size of the data in bytes
     */
    s_Event(const char* library, uint64_t event, void* _data, uint64_t dataSize)
     : type(library, event), data(_data, dataSize)
    {}

    /**
     * @brief Construct a new Event
     * 
     * @warning the size of the data to store in place is not sanity checked. It is not allowed to be greater than 16.
     * 
     * @param library the library the event came from
     * @param event the type of event
     * @param size the size of the data to store in place in bytes
     * @param inPlace a pointer to the data to store in place
     */
    s_Event(const char* library, uint64_t event, uint8_t size, byte* inPlace)
     : type(library, event), data(size, inPlace)
    {}

    /**
     * @brief Construct a new Event
     * 
     * @param _type the type of the event
     * @param _data the data for the event
     */
    s_Event(const EventType& _type, const EventData& _data)
     : type(_type), data(_data)
    {}

    /**
     * @brief check if two events are of the same type
     * 
     * @param e a constant reference to the event to compare with
     * @return true : the events are of the same type
     * @return false : the events are of different types
     */
    inline bool operator==(const s_Event& e) const noexcept 
    {return (!strcmp(type.library, e.type.library)) && (type.event == e.type.event);}

    #endif

} Event;

/**
 * @brief compare if two events are of the same type
 * 
 * @param first the first event to compare
 * @param second the event to compare the type with the first one
 * @return true : both events are of the same type
 * @return false : the events are not of the same type
 */
bool event_Compare(const Event first, const Event second);

#endif