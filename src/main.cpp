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

void preprocess(String* content, void*)
{
    for (size_t i = 0; i < content->size(); ++i)
    {
        if ((*content)[i] == 'f') {(*content)[i] = 't';}
    }
}

int main()
{
    SimpleVertex vertices[] = {
        SimpleVertex{
            .pos = vec3(0),
            .normal = (0,1,0),
            .tex = vec2(0,0)},
        SimpleVertex{
            .pos = vec3(1,0,0),
            .normal = vec3(0,1,0),
            .tex = vec2(1,0)},
        SimpleVertex{
            .pos = vec3(1,0,1),
            .normal = vec3(0,1,0),
            .tex = vec2(1,1)},
        SimpleVertex{
            .pos = vec3(0,0,1),
            .normal = vec3(0,1,0),
            .tex = vec2(0,1)}
    };

    Mesh mesh(vertices, sizeof(vertices)/sizeof(*vertices), GLGE_VERTEX_LAYOUT_SIMPLE_VERTEX, {0,1,2, 1,2,3});

    for (uint64_t i = 0; i < mesh.getVertexCount(); ++i)
    {
        SimpleVertex vert = mesh.getVertices<SimpleVertex>()[i];
        std::cout << vert.pos << ", " << vert.normal << ", " << vert.tex << "\n";
    }

    Sphere s(0, 1);
    std::cout << s.getVolume() << "\n";

    return 0;

    int level = -1;
    FileDecorator compress = {
        .preprocessor = glge_Decompress,
        .postprocessor = glge_Compress,
        .onOpen = NULL,
        .onClose = NULL,
        .userData = &level
    };
    String key_1 = "thiskeyisverybad";
    String key_2 = "dontusethisinput";
    CryptoInfo cInfo = {
        .key_1 = &key_1,
        .key_2 = &key_2
    };
    FileDecorator encrypt = {
        .preprocessor = glge_Decrypt,
        .postprocessor = glge_Encrypt,
        .onOpen = NULL,
        .onClose = NULL,
        .userData = &cInfo
    };
    File f("test.file", false, {encrypt, compress});
    printf("%s\n", f.getContents().c_str());
    f.getContents() = "Hello World!";
    f.save();
    
    return 0;
    
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

    Settings set;
    // set.setSetting_String("String", "Hello World!");
    // set.setSetting_Float("Lives", 0.4);
    // set.setSetting_Vec2("Position", vec2(32.5, 44.12));
    // set.saveToFile("settings.gsav");
    set.loadFromFile("settings.gsav");

    for (uint64_t i = 0; i < set.getSettingCount(); ++i)
    {
        std::cout << set.getSetting(i) << "\n";
    }

    app.run();
}