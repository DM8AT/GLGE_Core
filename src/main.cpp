#include <iostream>
#include "GLGECore/GLGECore.h"

#include <thread>
#include <unordered_map>
#include <string>


void handle(const Event* ev, void*)
{
    std::cout << ev->type.library << "\n";
}

int main()
{
    EventHandler handler(handle, NULL);
    handler.sendEvent(Event("Hello", 0));
    handler.sendEvent(Event("Test", 0));

    handler.handleEvents();
}