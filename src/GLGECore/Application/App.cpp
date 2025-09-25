/**
 * @file App.cpp
 * @author DM8AT
 * @brief implement the application system
 * @version 0.1
 * @date 2025-09-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the app system
#include "App.h"
//include pugixml for xml parsing
#include "pugixml.hpp"
//include file stuff
#include <filesystem>
#include <fstream>
//include the error handling
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

std::unordered_map<std::thread::id, App*> App::ms_appList = {};

/**
 * @brief load the app info structure from a document file
 * 
 * @param doc a reference to the document file to load the settings from
 * @return AppInfo the application's info as read in by the file (path = NULL)
 */
static AppInfo __loadAppInfoFromFile(pugi::xml_document& doc)
{
    //store the value to return
    AppInfo ret;
    //get the root node (the app node)
    pugi::xml_node app = doc.child("App");
    GLGE_ASSERT("Failed to load file " << std::filesystem::path(doc.path()) << " because no \"App\" element was found", !app);

    //load the App's name
    pugi::xml_attribute app_name = app.attribute("name");
    GLGE_ASSERT("Failed to load file " << std::filesystem::path(doc.path()) << " because no \"App::name\" element was found", !app_name);
    ret.name = app_name.value();

    //load the version
    if (pugi::xml_node version = app.child("Version"))
    {
        //load all elements from the version
        //all not present elements are set to 0

        //load the major element
        if (pugi::xml_node major = version.child("Major")) {
            //store the value
            if (pugi::xml_attribute major_value = major.attribute("value")) {
                //store the value
                ret.version.major = (uint16_t)atoi(major_value.value());
            }
            else {
                //warning
                GLGE_MESSAGE("[WARNING] The element \"App::Version::Major\" exists in file " << std::filesystem::path(doc.path()) << " but it does not have the element \"value\". Assuming 0.");
                ret.version.major = 0;
            }
        }
        else {
            ret.version.major = 0;
        }

        //load the minor element
        if (pugi::xml_node minor = version.child("Minor")) {
            //store the value
            if (pugi::xml_attribute minor_value = minor.attribute("value")) {
                //store the value
                ret.version.minor = (uint16_t)atoi(minor_value.value());
            }
            else {
                //warning
                GLGE_MESSAGE("[WARNING] The element \"App::Version::Minor\" exists in file " << std::filesystem::path(doc.path()) << " but it does not have the element \"value\". Assuming 0.");
                ret.version.minor = 0;
            }
        }
        else {
            ret.version.minor = 0;
        }

        //load the patch element
        if (pugi::xml_node patch = version.child("Patch")) {
            //store the value
            if (pugi::xml_attribute patch_value = patch.attribute("value")) {
                //store the value
                ret.version.patch = (uint16_t)atoi(patch_value.value());
            }
            else {
                //warning
                GLGE_MESSAGE("[WARNING] The element \"App::Version::Patch\" exists in file " << std::filesystem::path(doc.path()) << " but it does not have the element \"value\". Assuming 0.");
                ret.version.patch = 0;
            }
        }
        else {
            ret.version.patch = 0;
        }
    }
    else
    {
        //just store 0.0.0 as the version
        ret.version = {
            .major = 0,
            .minor = 0,
            .patch = 0
        };
    }

    //load the debug value. If it does not exist, assume false
    if (pugi::xml_node debug = app.child("Debug"))
    {
        //else, store the value
        if (pugi::xml_attribute debug_value = debug.attribute("value")) {
            //store the value
            //make sure to only store 1 and 0, as this makes debugging easier.
            //(if ret.debug would be something else, there somewhere is a memory error)
            if (atoi(debug_value.value())) {
                ret.debug = true;
            } else {
                ret.debug = false;
            }
        }
        else {
            //print a warning and assume false
            GLGE_MESSAGE("[WARNING] Element \"App::Debug\" exists in file " << std::filesystem::path(doc.path()) << ", but it does not have the element \"value\". Assuming \"false\"");
            ret.debug = false;
        }
    }
    else
    {
        //just set debugging to false
        ret.debug = false;
    }

    //return the application info
    return ret;
}

/**
 * @brief encode all the application info into an XML file
 * 
 * @param doc the document to store the application info to
 * @param info the application info to encode
 * @return true : successfully encoded everything
 * @return false : something went wrong
 */
static void __storeAppInfoToFile(pugi::xml_document& doc, const AppInfo& info) noexcept
{
    //store the XML data

    //General structure:
    // App node (name)
    // version node
    //  Major (value)
    //  Minor (value)
    //  Patch (value)
    // debug (value)

    //create the app node
    pugi::xml_node app = doc.append_child("App");
    //add the name
    pugi::xml_attribute app_name = app.append_attribute("name");
    app_name.set_value(info.name.c_str());

    //create the version node
    pugi::xml_node version = app.append_child("Version");
    //create the major version
    pugi::xml_node major = version.append_child("Major");
    pugi::xml_attribute major_value = major.append_attribute("value");
    major_value.set_value(std::to_string(info.version.major));
    //create the minor version
    pugi::xml_node minor = version.append_child("Minor");
    pugi::xml_attribute minor_value = minor.append_attribute("value");
    minor_value.set_value(std::to_string(info.version.minor));
    //create the patch version
    pugi::xml_node patch = version.append_child("Patch");
    pugi::xml_attribute patch_version = patch.append_attribute("value");
    patch_version.set_value(std::to_string(info.version.patch));

    //create the debug node
    pugi::xml_node debug = app.append_child("Debug");
    pugi::xml_attribute debug_value = debug.append_attribute("value");
    debug_value.set_value(std::to_string((uint8_t)info.debug));
}

void App::initialize() noexcept
{
    //set the settings path if it is NULL
    if (m_info.settingsPath == "") {m_info.settingsPath = "." + m_info.name + ".gapp";}
    //check if the file exists. If it does not exist, create it. 
    if (!std::filesystem::is_regular_file(m_info.settingsPath))
    {
        //setup a new file
        setupNewFile();
        return;
    }

    //load the app settings from a file
    setupFromFile();
}

void App::setupNewFile() noexcept
{
    //create the file
    pugi::xml_document doc;
    //store the app info data
    __storeAppInfoToFile(doc, m_info);
    
    //store the XML file
    GLGE_ASSERT("Failed to create XML file " << std::filesystem::path(m_info.settingsPath), 
                !doc.save_file(std::filesystem::path(m_info.settingsPath).c_str()))
}

void App::setupFromFile() noexcept
{
    //load the document
    pugi::xml_document doc;
    pugi::xml_parse_result res = doc.load_file(std::filesystem::path(m_info.settingsPath).c_str());

    //check the loading
    if (!res)
    {
        //print an error
        std::cerr << "[ERROR] Failed to load file " << std::filesystem::path(m_info.settingsPath) << ".\n    Error: " << res.description();
        abort();
    }

    //load the app info
    AppInfo info = __loadAppInfoFromFile(doc);

    //check the app's name to make sure the settings file belongs to the app
    if (m_info.name != info.name)
    {
        //critical error: tried to insert an application file belonging to another application
        GLGE_ABORT("[CRITICAL ERROR] The loaded application file registered as application " << info.name << ", but that does not match the actual application's name.");
    }

    //check the version. 0.0.0 means all version versions are allowed, else check depending on the version flag
    if (!((info.version.major == 0) && (info.version.minor == 0) && (info.version.patch == 0)))
    {
        //check the version against the own version depending on the flag

        //concatenate the version numbers
        uint64_t own_version = (((uint64_t)m_info.version.major) << (sizeof(uint16_t)*2)) | (((uint64_t)m_info.version.minor) << sizeof(uint16_t)) | ((uint64_t)m_info.version.patch);
        uint64_t oth_version = (((uint64_t)info.version.major)   << (sizeof(uint16_t)*2)) | (((uint64_t)info.version.minor)   << sizeof(uint16_t)) | ((uint64_t)info.version.patch);

        //check if the version is newer and newer is not supported
        if ((own_version < oth_version) && !(m_info.compatibility & APP_COMPATIBILITY_FORWARD)){
            //warning
            std::cerr << "[WARNING] Loaded the API version " << info.version.major << "." << info.version.minor << "." << info.version.patch << ", but forward compatibility was not enabled.\n";
        }
        //check if the version is older and backwards compatibility is not supported
        if ((own_version > oth_version) && !(m_info.compatibility & APP_COMPATIBILITY_BACKWARD)) {
            //warning
            std::cerr << "[WARNING] Loaded the API version " << info.version.major << "." << info.version.minor << "." << info.version.patch << ", but backward compatibility was not enabled.\n";
        }
    }

    //compare the debug values. If debug is requested in the file but not by the app, print a warning and overrule the file. 
    if (!m_info.debug && info.debug) {
        //warning!
        std::cerr << "[WARNING] The file requested debug access, but the app does not permit debugging. Overruling the file.\n";
    }
}

void App::run()
{
    //if no layers exist, the app will run endless. Prevent this. 
    if (m_layers.getLayerCount() == 0) {return;}

    //store this thread as the main thread
    m_threadNames[std::this_thread::get_id()] = "main";
    //register the app
    ms_appList[std::this_thread::get_id()] = this;

    //store that the app is active
    m_active = true;

    //initialize everything
    m_layers.signalStartup();

    //loop while the app is running
    while (m_active)
    {
        //update the layer stack
        m_layers.update();
    }

    //shutdown everything
    m_layers.signalShutdown();
}