/**
 * @file Scene.cpp
 * @author DM8AT
 * @brief implement complex functions for the scene
 * @version 0.1
 * @date 2025-10-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include what a scene is
#include "Scene.h"
//include systems (needed for calling functions on them)
#include "System.h"

//for remove_if
#include <algorithm>

Scene::~Scene()
{
    //delete all the systems added to the scene
    for (auto it = m_systems.begin(); it != m_systems.end(); ++it) {
        delete it->second;
        it->second = NULL;
    }
}

void Scene::deleteObject(Object object) noexcept {
    //Sanity check : don't delete root nor delete NULL
    if (object == NULL || object == ((Object)&m_root)) return; 

    //extract the object information
    RawObject* raw = (RawObject*)object;
    const std::string& name = raw->name;

    //re-parent the children
    if (raw->parent) {
        //the object is sane and has a parent
        for (RawObject* child : raw->children) {
            //change the parent and add the new child to the parent list
            ((RawObject*)child)->parent = raw->parent;
            raw->parent->children.push_back((RawObject*)child);
        }
    } else {
        //what?
        std::cerr << "Deleted an object that has no parent and is not root. This should not happen.\n";
        //just to prevent memory leaks delete all children
        while (raw->children.size() > 0) {
            deleteObject((Object)(&raw->children[0]));
        }
    }

    //remove from parent’s children list
    if (raw->parent) {
        auto& siblings = raw->parent->children;
        //erase the correct element
        siblings.erase(
            std::remove_if(siblings.begin(), siblings.end(),
                [&](RawObject* child) { return child == ((RawObject*)object); }),
            siblings.end()
        );
    }

    //destroy entity from the world
    mustache::Entity ent = *((mustache::Entity*)&raw->entity);
    m_world.entities().destroyNow(ent);

    //recycle name index if applicable
    size_t openParen = name.rfind('(');
    size_t closeParen = name.rfind(')');

    //check if the name has an index
    if (openParen != std::string::npos && closeParen == name.size() - 1 && openParen < closeParen) {
        //get the index from the name
        std::string baseName = name.substr(0, openParen);
        std::string numberStr = name.substr(openParen + 1, closeParen - openParen - 1);
        //put the removing in a try-cache - it very well may fail
        try {
            //if this actually is a number return it to the number pool
            uint64_t index = std::stoull(numberStr);
            m_nameFreeIndices[baseName].insert(index);

            //clean up map entries if no objects with that base name exist anymore
            if (m_objects.count(baseName) == 0 &&
                (m_nameFreeIndices[baseName].size() == m_nameUniquenessMap[baseName])) {
                m_nameFreeIndices.erase(baseName);
                m_nameUniquenessMap.erase(baseName);
            }
        } catch (...) {
            //if parsing fails, it's probably not a numbered name. Ignore.
        }
    }

    //remove from object map
    m_objects.erase(name);
}

void Scene::execute(ISystem* system) {
    system->execute(m_world);
}

void Scene::update() noexcept {
    //iterate over all systems and then update the map
    for (auto it = m_systems.begin(); it != m_systems.end(); ++it) 
    {execute(it->second);}
    //update the scene
    m_world.update();
}