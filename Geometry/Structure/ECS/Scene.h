/**
 * @file Scene.h
 * @author DM8AT
 * @brief define what a scene is. A scene will be defined as a collection of a world as well as some systems that operate on that world. 
 * @version 0.1
 * @date 2025-10-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_GEOMETRY_STRUCTURE_ECS_SCENE_
#define _GLGE_CORE_GEOMETRY_STRUCTURE_ECS_SCENE_

//include objects
#include "Object.h"
//transforms are added to all objects by default
#include "../Transform.h"

//for C++ create a scene class
#if __cplusplus

//include unordered maps for direct object access
#include <unordered_map>
//sets are for speeding up the duplicate name resolver
#include <set>

//mustache is the main ECS manager
#include "mustache/ecs/ecs.hpp"

//systems will be defined later
class ISystem;

/**
 * @brief a scene has a collection of objects. The only object possessed by the scene directly is the root object (fully empty object, no entity)
 */
class Scene {
public:

    /**
     * @brief Construct a new Scene
     * 
     * @param name the name of the scene
     */
    Scene(const char* name)
     : m_name(name)
    {}

    /**
     * @brief Construct a new Scene
     * 
     * @param name the name of the scene
     */
    Scene(const std::string& name)
     : m_name(name)
    {}

    /**
     * @brief Destroy the Scene
     */
    ~Scene();

    /**
     * @brief Get the Root object of the scene
     * 
     * @return const Object& a constant reference to the scene root
     */
    inline const RawObject& getRoot() const noexcept {return m_root;}

    /**
     * @brief make enough space for a specific amount of objects in a specific amount of objects
     * 
     * @param objectCount the amount of objects to prepare for
     * @param obj the object to prepare (NULL is interpreted as ROOT and is the default value)
     */
    void reserve(size_t objectCount, Object obj = NULL) noexcept {
        //if the object is NULL, select the root object
        if (obj) 
        {((RawObject*)obj)->children.reserve(objectCount);}
        else
        {m_root.children.reserve(objectCount);}
    }

    /**
     * @brief Create a new Object
     * 
     * @tparam Components a list of components to add to the entity. Every entity has at least an String storing the object's name
     * @param nameSuggestion the name SUGGESTION for the object
     * @param parent a pointer to the parent or NULL if the root should be used
     * @return Object* a pointer to the new object
     */
    template <typename ...Components> inline Object createObject(const String& nameSuggestion, Object parent = NULL) noexcept {
        //check if the name exists. If it does, get a number to add to the end to make it unique
        String name = nameSuggestion;
        if (m_objects.find(name) != m_objects.end())
        {
            //Name collision -> generate unique name from set
            auto& freeIndices = m_nameFreeIndices[nameSuggestion];
            uint64_t index;

            if (!freeIndices.empty()) {
                //reuse a previously freed index
                index = *freeIndices.begin();
                freeIndices.erase(freeIndices.begin());
            } else {
                //get next unused index
                index = ++m_nameUniquenessMap[nameSuggestion];
            }

            //format string
            name = nameSuggestion + "(" + std::to_string(index) + ")";
        }
        //store the object and add it to the internal world
        mustache::Entity ent = m_world.entities().create<String, Transform, Components...>();
        *(m_world.entities().getComponent<String>(ent)) = name;
        //add the new object to the object mapping and parent
        RawObject* par = (RawObject*)((parent) ? ((RawObject*)parent) : &m_root);
        m_objects.emplace(name, RawObject{
            .name = name,
            .entity = *((uint64_t*)&ent),
            .parent = par,
            .children{}
        });
        RawObject* newObj = &m_objects[name];
        par->children.push_back(newObj);
        //return a pointer to the new object
        return ((Object)newObj);
    }

    /**
     * @brief Create a lot of objects
     * 
     * @tparam Components the components to add to all objects
     * @param instances the amount of instances to create
     * @param nameSuggestion the suggestion for a name for all objects. The name is constructed as follows: nameSuggestion + ( + baseInstanceNumber+instanceID + )
     * @param parent a pointer to the parent object (NULL is interpreted as ROOT and is the default)
     * @return std::vector<Object> a list of all created objects
     */
    template <typename ...Components> 
    std::vector<Object> createObjects(uint64_t instances, const std::string& nameSuggestion, Object parent = NULL) noexcept
    {
        //get the non-null parent
        RawObject* par = (parent) ? ((RawObject*)parent) : &m_root;

        //make space for the objects
        std::vector<Object> ret;
        ret.reserve(instances);
        //make enough space in the parent
        par->children.reserve(par->children.size() + instances);

        //make space in the internal object map
        m_objects.reserve(m_objects.size() + instances);

        //now, create all objects
        for (size_t i = 0; i < instances; ++i) {
            //check if the name exists. If it does, get a number to add to the end to make it unique
            String name = nameSuggestion;
            if (m_objects.find(name) != m_objects.end())
            {
                //Name collision -> generate unique name from set
                auto& freeIndices = m_nameFreeIndices[nameSuggestion];
                uint64_t index;

                if (!freeIndices.empty()) {
                    //reuse a previously freed index
                    index = *freeIndices.begin();
                    freeIndices.erase(freeIndices.begin());
                } else {
                    //get next unused index
                    index = ++m_nameUniquenessMap[nameSuggestion];
                }

                //format string
                name = nameSuggestion + "(" + std::to_string(index) + ")";
            }

            //store the object and add it to the internal world
            mustache::Entity ent = m_world.entities().create<String, Transform, Components...>();
            *(m_world.entities().getComponent<String>(ent)) = name;
            //add the new object to the object mapping and parent
            m_objects.emplace(name, RawObject{
                .name = name,
                .entity = *((uint64_t*)&ent),
                .parent = par,
                .children{}
            });
            RawObject* newObj = &m_objects[name];
            par->children.push_back(newObj);
            ret.emplace_back((Object)newObj);
        }

        //return the objects
        return ret;
    }

    /**
     * @brief remove an object from the scene
     * 
     * @param object the object to remove
     */
    void deleteObject(Object object) noexcept;

    /**
     * @brief get a pointer to the requested component or NULL if the object does not have the requested component
     * 
     * @tparam Component the type of component to quarry
     * @param obj the object to quarry the component from
     * @return Component* a pointer to the component or NULL if it doesn't exist
     */
    template <typename Component>
    inline Component* get(const Object& obj) noexcept 
    {return m_world.entities().getComponent<Component>(*((mustache::Entity*)&obj->entity));}

    /**
     * @brief initialize a component in place for an object
     * 
     * @tparam Component the type of component to initialize
     * @tparam Args the argument types for the component constructor
     * @param obj the object to initialize the component for
     * @param args the arguments to pass to the component constructor
     * @return true : the object has the component
     * @return false : the object does not have the component
     */
    template <typename Component, typename ...Args>
    inline bool initialize(const Object& obj, Args... args) noexcept {
        //get the address of the component for the object
        Component* comp = get<Component>(obj);
        //if the component exists, call the constructor, else return false
        if (comp) {
            new (comp) Component(std::forward<Args>(args) ...);
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief check if an object with the name exists
     * 
     * @param name the name to check
     * @return true : an object with the name exists
     * @return false : the name is not used
     */
    inline bool exists(const std::string& name) noexcept {
        return m_objects.find(name) != m_objects.end();
    }

    /**
     * @brief get an object by the name
     * 
     * @param name the name of the object
     * @return Object the actual object (NULL if it is not found)
     */
    inline Object get(const std::string& name) noexcept {
        //quarry the object
        auto it = m_objects.find(name);
        //return the object (if it does not exist, return null)
        return (it == m_objects.end()) ? NULL : ((ObjectWrapper*)&it->second);
    }

    /**
     * @brief run a function on all objects in the scene
     * 
     * @tparam Components the arguments to pass to the function
     * @param func a function that takes in arguments of type Args and returns nothing
     * @param parallel true : all functions will execute in parallel | false : all functions will run on this thread
     */
    template <typename ...Components> inline void forAllObjects(void (*func)(Components ...args), bool parallel) noexcept 
    {m_world.entities().forEach(func, parallel ? mustache::JobRunMode::kParallel : mustache::JobRunMode::kCurrentThread);}

    /**
     * @brief get all objects that have a specific component as well as a pointer to the component
     * 
     * @tparam Component the component to search for
     * @return std::vector<std::pair<Object*, Component*>> a list of both a pointer to the object as well as a pointer to the component
     */
    template <typename Component> inline std::vector<std::pair<Object, Component*>> get() noexcept {
        //store a vector with enough space for potentially all entities
        std::vector<std::pair<Object,Component*>> out;
        out.reserve(m_objects.size());
        //get all entities of the type
        for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
            if (Component* comp = m_world.entities().getComponent<Component>(*((mustache::Entity*)&it->second.entity))) {
                out.emplace_back((const ObjectWrapper*)&it->second, comp);
            }
        }
        //remove not needed space and return
        out.shrink_to_fit();
        return out;
    }

    /**
     * @brief add a new system to the scene
     * 
     * @tparam T the system to add
     * @tparam Args the types for the arguments to pass to the constructor
     * @param args a list of arguments to parse to the constructor
     * @return true : successfully added the system
     * @return false : failed to add the system, it is allready added
     */
    template <class T, typename ...Args> bool addSystem(Args ...args) noexcept {
        //get the name of the type
        const char* name = typeid(T).name();
        //check if the system is allready added
        if (m_systems.find(name) != m_systems.end()) {
            //failed to add the system
            return false;
        }

        //add the new system
        m_systems.emplace(name, new T(args...));

        //success
        return true;
    }

    /**
     * @brief check if the scene has a specific system added
     * 
     * @tparam T the type of the system to check
     * @return true : the scene has a system of that type
     * @return false : the scene does not have a system of that type
     */
    template <class T> bool hasSystem() noexcept {
        //get the name of the type
        const char* name = typeid(T).name();
        //check if the system is contained
        return m_systems.find(name) != m_systems.end();
    }

    /**
     * @brief Get a system from the scene
     * 
     * @tparam T the type of system to quarry
     * @return T* a pointer to the system or NULL if the scene does not contain the system
     */
    template <class T> T* getSystem() noexcept {
        //get the name of the type
        const char* name = typeid(T).name();
        //get the system
        auto it = m_systems.find(name);
        //if the element exists, return it, else return NULL
        if (it == m_systems.end()) {return NULL;}
        return (T*)it->second;
    }

    /**
     * @brief clean up a system if the scene has it
     * 
     * @tparam T the type of system to remove
     */
    template <class T> void removeSystem() noexcept {
        //get the name of the type
        const char* name = typeid(T).name();
        //remove the element (if found)
        auto it = m_systems.find(name);
        if (it != m_systems.end()) {
            //delete the element to free the memory
            delete it->second;
            m_systems.erase(it);
        }
    }

    /**
     * @brief update the whole scene
     */
    void update() noexcept;

protected:

    /**
     * @brief run a single system
     * 
     * @param system a pointer to the system to run
     */
    void execute(ISystem* system);

    //store the name of the scene
    std::string m_name;

    //store a root object
    RawObject m_root = {
        .name = "ROOT",
        .entity = UINT64_MAX,
        .parent = NULL,
        .children{}
    };
    //store how often a name occurred. Only filled if the name occurred more than once. 
    std::unordered_map<std::string, uint64_t> m_nameUniquenessMap;
    //store all freed indices for the names
    std::unordered_map<std::string, std::set<uint64_t>> m_nameFreeIndices;
    //store all the objects
    std::unordered_map<std::string, RawObject> m_objects{{m_root.name, m_root}};
    //store the world of the scene
    mustache::World m_world;
    //store a mapping from the system's name to the system instance
    std::unordered_map<const char*, ISystem*> m_systems;

};

#endif

#endif