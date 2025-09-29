/**
 * @file App.h
 * @author DM8AT
 * @brief define an API for the application structure
 * @version 0.1
 * @date 2025-09-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_APP_
#define _GLGE_CORE_APP_

//include the layer system
#include "../Layers/Layers.h"
//include the setting stuff and the types
#include "../Setting/Setting.h"

/**
 * @brief store the version of an application
 * 
 * App version 0.0.0 is difcurredged as it stands for the NULL-version (a version that is ignored by the application structure)
 */
typedef struct s_AppVersion {
    //store the number for the major release version of the application
    uint16_t major;
    //store the number for the minor release version of the application
    uint16_t minor;
    //store the number for the patch version of the application
    uint16_t patch;
} AppVersion;

/**
 * @brief store an enum to use as a compatibility flag
 * 
 * They define how errors should be raised when loading a different version from an existing app file (excluding version 0.0.0)
 */
typedef enum e_AppCompatibility {
    //the app is compatable with no other versions of the app
    APP_COMPATIBILITY_NONE = 0b00,
    //the app is only compatable with newer app versions
    APP_COMPATIBILITY_FORWARD = 0b01,
    //the app is only compatable with older app versions
    APP_COMPATIBILITY_BACKWARD = 0b10,
    //the app is compatable with both older and newer app versions
    APP_COMPATIBILITY_FORWARD_BACKWARD = 0b11
} AppCompatibility;

/**
 * @brief store general information about an application
 */
typedef struct s_AppInfo {
    //store the name of the application
    String name;
    //store a file path to the settings file of the application. NULL is interpreted as the App's name + ".gapp" (GLGE App)
    String settingsPath;
    //store the version of the application
    AppVersion version;
    //store the compatibility flags
    uint8_t compatibility;
    //store if debugging is enabled
    bool debug;
} AppInfo;

#if __cplusplus

//include threading
#include <thread>
//include unordered maps
#include <unordered_map>
//include the C++ filesystem
#include <filesystem>

/**
 * @brief store a wrapper for the application system
 */
class App
{
public:

    /**
     * @brief Construct a new App
     * 
     * @param info the info for the application to create
     */
    inline App(const AppInfo& info)
     : m_info(info), m_layers(std::vector<Layer*>())
    {initialize();}

    /**
     * @brief Construct a new App
     * 
     * @param name the name of the application
     * @param settingsPath store a path to the settings file for the application
     * @param version the version of the application
     * @param compatibility define to which versions the app is compatable
     * @param debug a flag controlling if the application should start in debug or release mode
     */
    inline App(String name, String settingsPath = "", const AppVersion& version = {.major = 1,.minor = 0,.patch = 0}, uint8_t compatibility = APP_COMPATIBILITY_NONE, bool debug = false)
     : m_info({name, settingsPath, version, compatibility, debug}), m_layers(std::vector<Layer*>())
    {initialize();}

    /**
     * @brief Construct a new App
     * 
     * @param layers a vector of layers to create the layer stack from
     * @param name the name of the application
     * @param settingsPath store a path to the settings file for the application
     * @param version the version of the application
     * @param compatibility define to which versions the app is compatable
     * @param debug a flag controlling if the application should start in debug or release mode
     */
    inline App(const std::vector<Layer*>& layers, String name, String settingsPath = "", const AppVersion& version = {.major = 1,.minor = 0,.patch = 0}, uint8_t compatibility = APP_COMPATIBILITY_NONE, bool debug = false)
     : m_info({name, settingsPath, version, compatibility, debug}), m_layers(layers)
    {initialize();}

    /**
     * @brief Construct a new App
     * 
     * @param layers a vector of layers to create the layer stack from
     * @param name the name of the application
     * @param settingsPath store a path to the settings file for the application
     * @param version the version of the application
     * @param compatibility define to which versions the app is compatable
     * @param debug a flag controlling if the application should start in debug or release mode
     */
    inline App(const std::vector<LayerBase*>& layers, String name, String settingsPath = "", const AppVersion& version = {.major = 1,.minor = 0,.patch = 0}, uint8_t compatibility = APP_COMPATIBILITY_NONE, bool debug = false)
     : m_info({name, settingsPath, version, compatibility, debug}), m_layers(layers)
    {initialize();}

    /**
     * @brief Construct a new App
     * 
     * @param layers a vector of layers to create the layer stack from
     * @param name the name of the application
     * @param settingsPath store a path to the settings file for the application
     * @param version the version of the application
     * @param compatibility define to which versions the app is compatable
     * @param debug a flag controlling if the application should start in debug or release mode
     */
    inline App(const std::vector<LayerStackElement>& layers, String name, String settingsPath = "", const AppVersion& version = {.major = 1,.minor = 0,.patch = 0}, uint8_t compatibility = APP_COMPATIBILITY_NONE, bool debug = false)
     : m_info({name, settingsPath, version, compatibility, debug}), m_layers(layers)
    {initialize();}

    /**
     * @brief Construct a new App
     * 
     * @param layers a vector of layers to create the layer stack from
     * @param isLayerBase say if the vector contains layers or layer bases
     * @param name the name of the application
     * @param settingsPath store a path to the settings file for the application
     * @param version the version of the application
     * @param compatibility define to which versions the app is compatable
     * @param debug a flag controlling if the application should start in debug or release mode
     */
    inline App(const Vector& layers, bool isLayerBase, String name, String settingsPath = "", const AppVersion& version = {.major = 1,.minor = 0,.patch = 0}, uint8_t compatibility = APP_COMPATIBILITY_NONE, bool debug = false)
     : m_info({name, settingsPath, version, compatibility, debug}), m_layers(layers, isLayerBase)
    {initialize();}

    /**
     * @brief Get the Settings of the application
     * 
     * @return Settings& a reference to the settings of the application
     */
    inline Settings& getSettings() noexcept {return m_appSettings;}

    /**
     * @brief Get the Layer Stack of the application
     * 
     * @return LayerStack& a reference to the layer stack belonging to the application
     */
    inline LayerStack& getLayerStack() noexcept {return m_layers;}

    /**
     * @brief run the application till it is requested to exit
     * 
     * This function blocks the calling thread till the app is finished
     */
    void run();

    /**
     * @brief get if the app is currently active
     * 
     * @return true : the app is active
     * @return false : the app is not active
     */
    inline bool isActive() const noexcept {return m_active;}

    /**
     * @brief request the stopping of the application
     * 
     * This won't stop the app instantly. It will still finish this iteration, but will close then
     */
    inline void stop() noexcept {m_active = false;}

    /**
     * @brief Get the App belonging to this thread
     * 
     * @return App* a pointer to the app
     */
    inline static App* getApp() noexcept {return ms_appList[std::this_thread::get_id()];}

protected:

    /**
     * @brief shared initializer for the structure
     */
    void initialize() noexcept;

    /**
     * @brief initialize a new file
     */
    void setupNewFile() noexcept;

    /**
     * @brief initialize the app from a file
     */
    void setupFromFile() noexcept;

    //store information about the application
    AppInfo m_info;
    //store a layer stack for the application
    LayerStack m_layers;

    //store if the application is active
    bool m_active = false;

    //store the main thread for the application
    std::thread m_main;
    //store the thread name map
    std::unordered_map<std::thread::id, String> m_threadNames;

    //store the settings for the application
    Settings m_appSettings;

    /**
     * @brief store a mapping from thread ID's to the apps
     */
    static std::unordered_map<std::thread::id, App*> ms_appList;

};

//for C++ start a C section

extern "C" {

#else //else, use an opaque type

/**
 * @brief an opaque wrapper around an ap
 * 
 * The `unused` variable is actually UNUSED! DO NOT DEREFERENCE!
 */
typedef struct { uint8_t unused; } App;

#endif

/**
 * @brief Create a new app instance
 * 
 * @param info a pointer to some information to create the app from
 * @return App* a pointer to the new app object
 */
App* app_Create(const AppInfo* info);

/**
 * @brief Create a new app instance
 * 
 * @param name a pointer to a string that contains the app's name
 * @param settingPath a pointer to a string that contains a path to the location the app file is located / Will be located
 * @param version the version of the application
 * @param compatibility store a bitmask to define how the app is compatable
 * @param debug define if debugging is allowed
 * @return App* a pointer to the new app instance
 */
App* app_CreateFrom(const String* name, const String* settingPath, const AppVersion version, uint8_t compatibility, bool debug);

/**
 * @brief Create a new app instance
 * 
 * @param vector a pointer to a vector with layers for the app
 * @param isLayerBase store a boolean to define if the vector contains LayerBase pointer or Layer pointer (only important if pointers are used)
 * @param info a pointer to the information structure that contains the info on how to create the application
 * @return App* a pointer to the new app instance
 */
App* app_CreateWithLayers(const Vector* vector, bool isLayerBase, const AppInfo* info);

/**
 * @brief Create a new app instance
 * 
 * @param vector a pointer to a vector with layers for the app
 * @param isLayerBase store a boolean to define if the vector contains LayerBase pointer or Layer pointer (only important if pointers are used)
 * @param name a pointer to a string that contains the app's name
 * @param settingPath a pointer to a string that contains a path to the location the app file is located / Will be located
 * @param version the version of the application
 * @param compatibility store a bitmask to define how the app is compatable
 * @param debug define if debugging is allowed
 * @return App* a pointer to the new app instance
 */
App* app_CreateFromWithLayers(const Vector* vector, bool isLayerBase, const String* name, const String* settingPath, const AppVersion version, uint8_t compatibility, bool debug);

/**
 * @brief delete an app instance
 * 
 * @param app a pointer to the instance to delete
 */
void app_Delete(App* app);

/**
 * @brief get a pointer to the settings stored in the application
 * 
 * @param app a pointer to the app to read the settings from
 * @return Settings* a pointer to the settings object that belongs to the app
 */
Settings* app_GetSettings(App* app);

/**
 * @brief get the layer stack that belongs to the application
 * 
 * @param app a pointer to the app to quarry the layer stack from
 * @return LayerStack* a pointer to the layer stack contained by the app
 */
LayerStack* app_GetLayerStack(App* app);

/**
 * @brief run an app structure
 * 
 * @param app a pointer to the app instance to run
 */
void app_Run(App* app);

/**
 * @brief check if an application is running
 * 
 * @param app a pointer to the application to check
 * @return true : the application is currently running
 * @return false : the application is currently not running
 */
bool app_IsActive(const App* app);

/**
 * @brief stop the application if it is running
 * 
 * @param app a pointer to the application to stop
 */
void app_Stop(App* app);

/**
 * @brief get the application that uses this thread as its main thread
 * 
 * @return App* a pointer to the application that owns this thread or NULL if none does
 */
App* app_GetApp();

//end the C section
#if __cplusplus
}
#endif

#endif