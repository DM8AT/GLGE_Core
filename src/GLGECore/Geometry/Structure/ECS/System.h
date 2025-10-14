/**
 * @file System.h
 * @author DM8AT
 * @brief Define systems (C++ only)
 * @version 0.1
 * @date 2025-10-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_GEOMETRY_STRUCTURE_ECS_SYSTEM_
#define _GLGE_CORE_GEOMETRY_STRUCTURE_ECS_SYSTEM_

//the stuff is only available for C++
#if __cplusplus

//include the mustache ECS
#include "mustache/ecs/ecs.hpp"

/**
 * @brief a common base class for all systems
 */
class ISystem {
public:

    /**
     * @brief store what settings for the system mean
     */
    struct Settings {
        bool parallel = false;
        bool active = true;
    };

    /**
     * @brief Construct a new ISystem
     * 
     * @param settings the settings to create
     */
    ISystem(const Settings& settings = Settings{false, true})
     : m_system_settings(settings)
    {}

    /**
     * @brief Destroy the ISystem
     */
    virtual ~ISystem() {}

    /**
     * @brief a function to call per update
     */
    virtual void onUpdate() = 0;

    /**
     * @brief a function to call on startup
     */
    virtual void onInit() = 0;

    /**
     * @brief a function to call on closing
     */
    virtual void onDeinit() = 0;

    /**
     * @brief initialize the system
     */
    virtual void init() noexcept final {if (m_system_settings.active) {onInit();}}

    /**
     * @brief update the system
     */
    virtual void update() noexcept final {if (m_system_settings.active) {onUpdate();}}

    /**
     * @brief de-init the system
     */
    virtual void deinit() noexcept final {if (m_system_settings.active) {onDeinit();}}

    /**
     * @brief run the system
     */
    virtual void execute(mustache::World& world) noexcept = 0;

protected:

    /**
     * @brief store the settings for the system
     */
    Settings m_system_settings;

};

/**
 * @brief a simple wrapper for systems
 * 
 * @tparam T the type of system to create
 */
template <class T> class System : public ISystem, public mustache::PerEntityJob<T> {
public:

    /**
     * @brief Construct a new System
     * 
     * @param settings the settings for the system
     */
    System(const ISystem::Settings& settings = ISystem::Settings{false, true})
     : ISystem(settings)
    {}

    /**
     * @brief execute the system in an entity world
     * 
     * @param world the world to execute in
     */
    virtual void execute(mustache::World& world) noexcept final override 
    {((T*)(this))->run(world, m_system_settings.parallel ? mustache::JobRunMode::kParallel : mustache::JobRunMode::kCurrentThread);}

};

#endif

#endif