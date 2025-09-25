#include <iostream>
#include "GLGECore/GLGECore.h"

class LayerClass : public LayerBase
{
public:

    LayerClass(const String& _name)
     : LayerBase("Main", "Test", onEvent), name(_name)
    {}

    virtual void onSetup() override
    {std::cout << name << " : Setup\n";}

    virtual void onUpdate() override
    {std::cout << name << " : Update\n";}

    virtual void onShutdown() override
    {std::cout << name << " : Shutdown\n";}

protected:

    static void onEvent(const Event* ev, void*)
    {std::cout << "Received an event\n    Library: " << ev->type.library << "\n    Event Type: " << ev->type.event << "\n";}

    String name;

};

void stackFunc_update()
{
    App::getApp()->stop();
    std::cout << "Updated\n";
}

int main()
{
    App app({
        new Layer(
            "Main", "Second Test", {
                .next = NULL,
                .func_updateCallback = stackFunc_update,
                .func_startupCallback = NULL,
                .func_shutdownCallback = NULL
            }
        ),
        new LayerClass("Hello")
    }, "TestApp");

    app.run();
}