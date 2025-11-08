/**
 * @file Object.h
 * @author DM8AT
 * @brief an object is a single element in the scene. It can have components attached. By default, it has a transform component. An object has a parent (unless it is root) and may have multiple children. An object has an unique name. 
 * @version 0.1
 * @date 2025-10-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_GEOMETRY_STRUCTURE_ECS_OBJECT_
#define _GLGE_CORE_GEOMETRY_STRUCTURE_ECS_OBJECT_

//include types for all int-related types
#include "../../../Types.h"
//include strings for names
#include "../../../../GLGE_BG/CBinding/String.h"

//the maximum depth an object print may reach before exiting preemitfly
#define OBJECT_PRINT_DEPTH_LIMIT 32

//include unordered maps for C++ only
#if __cplusplus
    //vectors are needed for raw objects
    #include <vector>
    //add the input/output system
    #include <iostream>

    //scenes will be defined later
    class Scene;

    /**
     * @brief store what a raw object actually is (opaque for most of the time)
     */
    struct RawObject {
        //the unique (within the scene) name of the object
        std::string name = "INVALID";
        //the identifier of the object's entity
        uint64_t entity = UINT64_MAX;
        //store a pointer to the scene the object belongs to
        Scene* scene = nullptr;
        //store a pointer to the parent (NULL = this is root)
        RawObject* parent = NULL;
        //store a vector of object pointers to the children
        std::vector<RawObject*> children{};

        /**
         * @brief recursively print the object with all children
         * 
         * @param os the output stream to print to
         * @param depth the current depth
         * @return std::ostream& the filled output stream
         */
        std::ostream& print(std::ostream& os, uint64_t depth) const noexcept {
            //compute the indent
            std::string indent = std::string(2*depth, ' ');
            //depth check to stop endless recursion
            if (depth > OBJECT_PRINT_DEPTH_LIMIT) {
                //add a symbol to show that the print stopped early and return
                os << indent << "[...]\n";
                return os;
            }
            
            //print the name at the correct indent
            os << indent << name << "\n";
            //only print the children if the element has children
            if (children.size()) {
                for (size_t i = 0; i < children.size(); ++i) {
                    (void)children[i]->print(os, depth+1);
                }
            }
            //return the filled stream
            return os;
        }

        /**
         * @brief print an object and all children to an output stream
         * 
         * @param os the output stream to print to
         * @param obj the object to print
         * @return std::ostream& the filled output stream
         */
        inline friend std::ostream& operator<<(std::ostream& os, const RawObject& obj) noexcept 
        {return obj.print(os, 0);}
    };

#endif

/**
 * @brief wrap a raw object for C
 * 
 * Some parts are left opaque for a reason
 */
typedef struct s_ObjectWrapper {
    //store the name of the object
    String name;
    //opaque entity type
    uint64_t entity;
    //store a pointer to the scene the object belongs to
    void* scene;
    //parent pointer
    void* parent;
    //opaque padding
    byte padding[24];


    //for C++ add helper functions
    #if __cplusplus

    /**
     * @brief access a specific component of the object
     * 
     * @tparam Component the type of component to get
     * @return Component* a pointer to the component or NULL if the component does not exist
     */
    template <typename Component> inline Component* get() noexcept;

    /**
     * @brief initialize a component of the object
     * 
     * @tparam Component the component to initialize
     * @tparam Args the types of arguments to parse to the constructor
     * @param args the actual arguments to parse to the constructor
     * @return true : the initialization was called
     * @return false : the requested component is not attached to the object
     */
    template <typename Component, typename ...Args> inline bool initialize(Args... args) noexcept;

    /**
     * @brief check if this object has a specific component
     * 
     * @tparam Component the component to check
     * @return true : this object has the requested component
     * @return false : this object does NOT have the requested component
     */
    template <typename Component> inline bool has() noexcept;

    /**
     * @brief assign the value of a component or add the component if it doesn't exist
     * 
     * @tparam Component the component to assign or create
     * @tparam Args the arguments to parse to the constructor / assignment
     * @param args the arguments to parse to the constructor / assignment
     */
    template <typename Component, typename ...Args> inline void assignOrAdd(Args... args) noexcept;

    /**
     * @brief remove a component from this object
     * 
     * @tparam Component the component to remove
     */
    template <typename Component> inline void remove() noexcept;

    #endif

} ObjectWrapper;

//define what an object actually is
typedef ObjectWrapper* Object;

//for C++ add a print operator to the object
#if __cplusplus

//overload the print operator for the Object object
inline std::ostream& operator<<(std::ostream& os, Object object) noexcept 
{return ((RawObject*)object)->print(os, 0);}

#endif

#endif