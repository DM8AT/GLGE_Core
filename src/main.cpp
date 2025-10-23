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

class SimpleUpdate : public System<SimpleUpdate> {
public:
    
    SimpleUpdate()
     : System<SimpleUpdate>({
            .parallel = true,
            .active = true
        })
    {}

    void operator()(Transform& trans) {
        trans.pos.x = 5;
    }

    virtual void onInit() noexcept override {}
    virtual void onUpdate() noexcept override {}
    virtual void onDeinit() noexcept override {}
};

template <typename T> struct Foo {Foo();}

void perObjectFunction(Transform& trans) {
    trans.pos.y += 5;
}

int main()
{
    Foo<AssetTypeRegistry::getID<int>()> foo;

    Scene scene = "Main Scene";
    scene.addSystem<SimpleUpdate>();
    std::vector<Object> objs;

    {
        ScopeTimer s = "Object Creation";
        objs.reserve(1E6);
        objs = scene.createObjects(objs.capacity(), "Hi");
    }

    {
        ScopeTimer s = "Systems";
        scene.update();
    }

    {
        ScopeTimer s = "Per object function";
        scene.forAllObjects(perObjectFunction, false);
    }

    return 0;

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

    std::vector<Triangle> tries;
    tries.reserve(12);
    for (uint64_t i = 0; i < tries.capacity(); ++i) {tries.emplace_back(vec3(0), vec3(1), vec3(0,1,0));}
    BVH<AABB, Triangle, 8> bvh(tries);

    std::cout << bvh;

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