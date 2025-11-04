/**
 * @file Event.cpp
 * @author DM8AT
 * @brief implement the C binding for event member functions
 * @version 0.1
 * @date 2025-09-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the event API
#include "Event.h"

bool event_Compare(const Event first, const Event second)
{return (first == second);}