/**
 * @file Settings.h
 * @author DM8AT
 * @brief define a simple interface for a structure to store and load settings
 * @version 0.1
 * @date 2025-09-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_SETTINGS_
#define _GLGE_CORE_SETTINGS_

//include types
#include "../Types.h"
//include strings
#include "../../GLGE_BG/CBinding/String.h"
//include the math types
#include "../../GLGE_Math/GLGEMath.h"

/**
 * @brief store an enum for the app setting
 */
typedef enum e_SettingType {
    //integer types
    SETTING_TYPE_UINT8 = 0,
    SETTING_TYPE_INT8,
    SETTING_TYPE_UINT16,
    SETTING_TYPE_INT16,
    SETTING_TYPE_UINT32,
    SETTING_TYPE_INT32,
    SETTING_TYPE_UINT64,
    SETTING_TYPE_INT64,
    //float types
    SETTING_TYPE_FLOAT,
    SETTING_TYPE_DOUBLE,
    //strings
    SETTING_TYPE_STRING,
    //float vectors
    SETTING_TYPE_VEC2,
    SETTING_TYPE_VEC3,
    SETTING_TYPE_VEC4,
    //double vectors
    SETTING_TYPE_DVEC2,
    SETTING_TYPE_DVEC3,
    SETTING_TYPE_DVEC4,
    //uint32_t vectors
    SETTING_TYPE_UIVEC2,
    SETTING_TYPE_UIVEC3,
    SETTING_TYPE_UIVEC4,
    //int32_t vectors
    SETTING_TYPE_IVEC2,
    SETTING_TYPE_IVEC3,
    SETTING_TYPE_IVEC4
} SettingType;

/**
 * @brief store only the value of an app setting
 */
typedef union u_SettingValue {
    //integer types
    uint8_t  _uint8_t;
    int8_t   _int8_t;
    uint16_t _uint16_t;
    int16_t  _int16_t;
    uint32_t _uint32_t;
    int32_t  _int32_t;
    uint64_t _uint64_t;
    int64_t  _int64_t;
    //float types
    float    _float;
    double   _double;
    //String (optional, 0 means unset)
    String  _string;
    //float vectors
    vec2     _vec2;
    vec3     _vec3;
    vec4     _vec4;
    //double vectors
    dvec2    _dvec2;
    dvec3    _dvec3;
    dvec4    _dvec4;
    //uint32_t vectors
    uivec2   _uivec2;
    uivec3   _uivec3;
    uivec4   _uivec4;
    //int32_t vectors
    ivec2    _ivec2;
    ivec3    _ivec3;
    ivec4    _ivec4;

    //for C++ add a default constructor
    #if __cplusplus

        /**
         * @brief Construct a new App Setting Value
         */
        u_SettingValue() noexcept : _string() {}

        /**
         * @brief Destroy the App Setting Value
         */
        ~u_SettingValue() {}

        /**
         * @brief set the value of the union
         * 
         * @tparam T the type of element to set
         */
        template <typename T> void operator=(const T&) noexcept;

        //check for PugiXML to add encode functionality
        #if PUGIXML_VERSION

        /**
         * @brief serialize the union to a node for XML
         * 
         * @tparam type the type of the union to encode
         * @param parent a reference to the parent node
         * @param name the name of the element to store
         */
        template <SettingType type> void encode(pugi::xml_node& parent) const;

        /**
         * @brief decode the setting from an XML node
         * 
         * @param value a reference to the XML node to decode the value from
         * @param type the integer identifier for the value's type
         * @return SettingType the actual type of the setting
         */
        SettingType decode(pugi::xml_node& value, uint64_t type);

    private:

        /**
         * @brief decode the setting with a specific type from a node
         * 
         * @tparam type the type of the setting to decode
         * @param value a reference to the node to decode the value from
         */
        template <SettingType type> void __decodeType(pugi::xml_node& value);

    public:

        #endif //end of PugiXML only section

    #endif

} SettingValue;

/**
 * @brief store a single setting element for the application
 */
typedef struct s_Setting {
    //store what type of value is stored
    SettingType type;
    //store the value of the setting
    SettingValue value;

    //only implement some functions for C++
    #if __cplusplus

    /**
     * @brief print the settings to an output stream
     * 
     * @param os the output stream to fill
     * @param setting the settings to print
     * @return std::ostream& the filled output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const s_Setting& setting) noexcept;

private:

    /**
     * @brief print this setting into an output stream
     * 
     * @tparam type the type of setting to print
     * @param os the output stream to fill
     * @return std::ostream& the filled output stream
     */
    template <SettingType type> std::ostream& printSelf(std::ostream& os) const;

public:

    #endif

} Setting;

/**
 * @brief the same as the app setting structure, but also containing a string "name" in the first spot
 */
typedef struct s_SettingNamed {
    //store the name of the setting
    String name;
    //store what type of value is stored
    SettingType type;
    //store the value of the setting
    SettingValue value;
} SettingNamed;

//check for C++ to create a class
#if __cplusplus

//include unordered maps for fast string -> value mapping
#include <unordered_map>

/**
 * @brief store a map from names to values
 */
class Settings
{
public:

    /**
     * @brief Construct Settings
     */
    Settings() = default;

    /**
     * @brief Destroy the Settings
     */
    ~Settings() {}

    /**
     * @brief store the settings to a file
     * 
     * @param file the path to store the settings to
     */
    void saveToFile(const char* file);

    /**
     * @brief load the settings from a file
     * 
     * The settings are stored in an element called "Settings"
     * 
     * @param file the name of the file to load from
     */
    void loadFromFile(const char* file);

    /**
     * @brief Get the amount of settings currently loaded
     * 
     * @return uint64_t the amount of loaded setting elements
     */
    inline uint64_t getSettingCount() const noexcept {return m_settings.size();}

    /**
     * @brief Get the Setting Name of a setting element at a specific index
     * 
     * @param index the index of the element to quarry
     * @return const String& the name of the element
     */
    inline const String& getSettingName(uint64_t index) const noexcept {auto it = m_settings.begin(); std::advance(it, index); return it->first;}

    /**
     * @brief Get the Settings of a setting element at a specific value
     * 
     * @param index the index of the element to quarry
     * @return const Setting& a constant reference to the element at that index
     */
    inline const Setting& getSetting(uint64_t index) const noexcept {auto it = m_settings.begin(); std::advance(it, index); return it->second;}

/*
 $$$$$$\  $$$$$$$$\ $$$$$$$$\ $$$$$$$$\ $$$$$$\ $$\   $$\  $$$$$$\  
$$  __$$\ $$  _____|\__$$  __|\__$$  __|\_$$  _|$$$\  $$ |$$  __$$\ 
$$ /  \__|$$ |         $$ |      $$ |     $$ |  $$$$\ $$ |$$ /  \__|
\$$$$$$\  $$$$$\       $$ |      $$ |     $$ |  $$ $$\$$ |$$ |$$$$\ 
 \____$$\ $$  __|      $$ |      $$ |     $$ |  $$ \$$$$ |$$ |\_$$ |
$$\   $$ |$$ |         $$ |      $$ |     $$ |  $$ |\$$$ |$$ |  $$ |
\$$$$$$  |$$$$$$$$\    $$ |      $$ |   $$$$$$\ $$ | \$$ |\$$$$$$  |
 \______/ \________|   \__|      \__|   \______|\__|  \__| \______/ 
*/

    /**
     * @brief Set a single setting element to a signed 8 bit integer
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_Int8(const String& name, const int8_t& value) noexcept {setSetting<int8_t, SETTING_TYPE_INT8>(name, value);}

    /**
     * @brief Set a single setting element to an unsigned 8 bit integer
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_Uint8(const String& name, const uint8_t& value) noexcept {setSetting<uint8_t, SETTING_TYPE_UINT8>(name, value);}

    /**
     * @brief Set a single setting element to a signed 16 bit integer
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_Int16(const String& name, const int16_t& value) noexcept {setSetting<int16_t, SETTING_TYPE_INT16>(name, value);}

    /**
     * @brief Set a single setting element to an unsigned 16 bit integer
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_Uint16(const String& name, const uint16_t& value) noexcept {setSetting<uint16_t, SETTING_TYPE_UINT16>(name, value);}

    /**
     * @brief Set a single setting element to a signed 32 bit integer
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_Int32(const String& name, const int32_t& value) noexcept {setSetting<int32_t, SETTING_TYPE_INT32>(name, value);}

    /**
     * @brief Set a single setting element to an unsigned 32 bit integer
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_Uint32(const String& name, const uint32_t& value) noexcept {setSetting<uint32_t, SETTING_TYPE_UINT32>(name, value);}

    /**
     * @brief Set a single setting element to a signed 64 bit integer
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_Int64(const String& name, const int64_t& value) noexcept {setSetting<int64_t, SETTING_TYPE_INT64>(name, value);}

    /**
     * @brief Set a single setting element to an unsigned 64 bit integer
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_Uint64(const String& name, const uint64_t& value) noexcept {setSetting<uint64_t, SETTING_TYPE_UINT64>(name, value);}

    /**
     * @brief Set a single setting element to a 32 bit IEEE float
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_Float(const String& name, const float& value) noexcept {setSetting<float, SETTING_TYPE_FLOAT>(name, value);}

    /**
     * @brief Set a single setting element to 64 bit IEEE float (double)
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_Double(const String& name, const double& value) noexcept {setSetting<double, SETTING_TYPE_DOUBLE>(name, value);}

    /**
     * @brief Set a single setting element to a string
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_String(const String& name, const String& value) noexcept {setSetting<String, SETTING_TYPE_STRING>(name, value);}

    /**
     * @brief Set a single setting element to a 2 element vector of floats
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_Vec2(const String& name, const vec2& value) noexcept {setSetting<vec2, SETTING_TYPE_VEC2>(name, value);}

    /**
     * @brief Set a single setting element to a 3 element vector of floats
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_Vec3(const String& name, const vec3& value) noexcept {setSetting<vec3, SETTING_TYPE_VEC3>(name, value);}

    /**
     * @brief Set a single setting element to a 4 element vector of floats
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_Vec4(const String& name, const vec4& value) noexcept {setSetting<vec4, SETTING_TYPE_VEC4>(name, value);}

    /**
     * @brief Set a single setting element to a 2 element vector of doubles
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_DVec2(const String& name, const dvec2& value) noexcept {setSetting<dvec2, SETTING_TYPE_DVEC2>(name, value);}

    /**
     * @brief Set a single setting element to a 3 element vector of doubles
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_DVec3(const String& name, const dvec3& value) noexcept {setSetting<dvec3, SETTING_TYPE_DVEC3>(name, value);}

    /**
     * @brief Set a single setting element to a 4 element vector of doubles
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_DVec4(const String& name, const dvec4& value) noexcept {setSetting<dvec4, SETTING_TYPE_DVEC4>(name, value);}

    /**
     * @brief Set a single setting element to a 2 element vector of unsigned 32 bit integers
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_UIVec2(const String& name, const uivec2& value) noexcept {setSetting<uivec2, SETTING_TYPE_UIVEC2>(name, value);}

    /**
     * @brief Set a single setting element to a 3 element vector of unsigned 32 bit integers
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_UIVec3(const String& name, const uivec3& value) noexcept {setSetting<uivec3, SETTING_TYPE_UIVEC3>(name, value);}

    /**
     * @brief Set a single setting element to a 4 element vector of unsigned 32 bit integers
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_UIVec4(const String& name, const uivec4& value) noexcept {setSetting<uivec4, SETTING_TYPE_UIVEC4>(name, value);}

    /**
     * @brief Set a single setting element to a 2 element vector of signed 32 bit integers
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_IVec2(const String& name, const ivec2& value) noexcept {setSetting<ivec2, SETTING_TYPE_IVEC2>(name, value);}

    /**
     * @brief Set a single setting element to a 3 element vector of signed 32 bit integers
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_IVec3(const String& name, const ivec3& value) noexcept {setSetting<ivec3, SETTING_TYPE_IVEC3>(name, value);}

    /**
     * @brief Set a single setting element to a 4 element vector of signed 32 bit integers
     * 
     * @param name the name of the setting to change / initialize
     * @param value the new value for the value
     */
    inline void setSetting_IVec4(const String& name, const ivec4& value) noexcept {setSetting<ivec4, SETTING_TYPE_IVEC4>(name, value);}

/*
 $$$$$$\  $$$$$$$$\ $$$$$$$$\ $$$$$$$$\ $$$$$$\ $$\   $$\  $$$$$$\  
$$  __$$\ $$  _____|\__$$  __|\__$$  __|\_$$  _|$$$\  $$ |$$  __$$\ 
$$ /  \__|$$ |         $$ |      $$ |     $$ |  $$$$\ $$ |$$ /  \__|
$$ |$$$$\ $$$$$\       $$ |      $$ |     $$ |  $$ $$\$$ |$$ |$$$$\ 
$$ |\_$$ |$$  __|      $$ |      $$ |     $$ |  $$ \$$$$ |$$ |\_$$ |
$$ |  $$ |$$ |         $$ |      $$ |     $$ |  $$ |\$$$ |$$ |  $$ |
\$$$$$$  |$$$$$$$$\    $$ |      $$ |   $$$$$$\ $$ | \$$ |\$$$$$$  |
 \______/ \________|   \__|      \__|   \______|\__|  \__| \______/ 
*/

    /**
     * @brief Get the state of a setting with the type of an 8 bit signed integer
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline int8_t* getSetting_Int8(const String& name) noexcept {return getSetting<int8_t, SETTING_TYPE_INT8>(name);}

    /**
     * @brief Get the state of a setting with the type of an 8 bit unsigned integer
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline uint8_t* getSetting_UInt8(const String& name) noexcept {return getSetting<uint8_t, SETTING_TYPE_UINT8>(name);}

    /**
     * @brief Get the state of a setting with the type of an 16 bit signed integer
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline int16_t* getSetting_Int16(const String& name) noexcept {return getSetting<int16_t, SETTING_TYPE_INT16>(name);}

    /**
     * @brief Get the state of a setting with the type of an 16 bit unsigned integer
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline uint16_t* getSetting_UInt16(const String& name) noexcept {return getSetting<uint16_t, SETTING_TYPE_UINT16>(name);}

    /**
     * @brief Get the state of a setting with the type of an 32 bit signed integer
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline int32_t* getSetting_Int32(const String& name) noexcept {return getSetting<int32_t, SETTING_TYPE_INT32>(name);}

    /**
     * @brief Get the state of a setting with the type of an 32 bit unsigned integer
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline uint32_t* getSetting_UInt32(const String& name) noexcept {return getSetting<uint32_t, SETTING_TYPE_UINT32>(name);}

    /**
     * @brief Get the state of a setting with the type of an 64 bit signed integer
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline int64_t* getSetting_Int64(const String& name) noexcept {return getSetting<int64_t, SETTING_TYPE_INT64>(name);}

    /**
     * @brief Get the state of a setting with the type of an 64 bit unsigned integer
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline uint64_t* getSetting_UInt64(const String& name) noexcept {return getSetting<uint64_t, SETTING_TYPE_UINT64>(name);}

    /**
     * @brief Get the state of a setting with the type of a 32 bit IEEE float
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline float* getSetting_Float(const String& name) noexcept {return getSetting<float, SETTING_TYPE_FLOAT>(name);}

    /**
     * @brief Get the state of a setting with the type of a 64 bit IEEE float (double)
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline double* getSetting_Double(const String& name) noexcept {return getSetting<double, SETTING_TYPE_DOUBLE>(name);}

    /**
     * @brief Get the state of a setting with the type of a string
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline String* getSetting_String(const String& name) noexcept {return getSetting<String, SETTING_TYPE_STRING>(name);}

    /**
     * @brief Get the state of a setting with the type of a float vector with 2 elements
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline vec2* getSetting_Vec2(const String& name) noexcept {return getSetting<vec2, SETTING_TYPE_VEC2>(name);}

    /**
     * @brief Get the state of a setting with the type of a float vector with 3 elements
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline vec3* getSetting_Vec3(const String& name) noexcept {return getSetting<vec3, SETTING_TYPE_VEC3>(name);}

    /**
     * @brief Get the state of a setting with the type of a float vector with 4 elements
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline vec4* getSetting_Vec4(const String& name) noexcept {return getSetting<vec4, SETTING_TYPE_VEC4>(name);}

    /**
     * @brief Get the state of a setting with the type of a double vector with 2 elements
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline dvec2* getSetting_DVec2(const String& name) noexcept {return getSetting<dvec2, SETTING_TYPE_DVEC2>(name);}

    /**
     * @brief Get the state of a setting with the type of a double vector with 3 elements
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline dvec3* getSetting_DVec3(const String& name) noexcept {return getSetting<dvec3, SETTING_TYPE_DVEC3>(name);}

    /**
     * @brief Get the state of a setting with the type of a double vector with 4 elements
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline dvec4* getSetting_DVec4(const String& name) noexcept {return getSetting<dvec4, SETTING_TYPE_DVEC4>(name);}

    /**
     * @brief Get the state of a setting with the type of a unsigned 32 bit integer vector with 2 elements
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline uivec2* getSetting_UIVec2(const String& name) noexcept {return getSetting<uivec2, SETTING_TYPE_UIVEC2>(name);}

    /**
     * @brief Get the state of a setting with the type of a unsigned 32 bit integer vector with 3 elements
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline uivec3* getSetting_UIVec3(const String& name) noexcept {return getSetting<uivec3, SETTING_TYPE_UIVEC3>(name);}

    /**
     * @brief Get the state of a setting with the type of a unsigned 32 bit integer vector with 4 elements
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline uivec4* getSetting_UIVec4(const String& name) noexcept {return getSetting<uivec4, SETTING_TYPE_UIVEC4>(name);}

    /**
     * @brief Get the state of a setting with the type of a signed 32 bit integer vector with 2 elements
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline ivec2* getSetting_IVec2(const String& name) noexcept {return getSetting<ivec2, SETTING_TYPE_IVEC2>(name);}

    /**
     * @brief Get the state of a setting with the type of a signed 32 bit integer vector with 3 elements
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline ivec3* getSetting_IVec3(const String& name) noexcept {return getSetting<ivec3, SETTING_TYPE_IVEC3>(name);}

    /**
     * @brief Get the state of a setting with the type of a signed 32 bit integer vector with 4 elements
     * 
     * @param name the name of the setting to quarry
     * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
     */
    inline ivec4* getSetting_IVec4(const String& name) noexcept {return getSetting<ivec4, SETTING_TYPE_IVEC4>(name);}

protected:

    /**
     * @brief Set a single setting of the application
     * 
     * @tparam T the type of setting to store
     * @tparam type the type identifier
     * @param name the name for the setting
     * @param value the value for the setting
     */
    template <typename T, SettingType type> void setSetting(const String& name, const T& value) noexcept;

    /**
     * @brief Get a single setting of the application
     * 
     * @tparam T the type of setting to store
     * @tparam type the type identifier
     * @param name the name for the setting to quarry
     * @return T* a pointer to the read value. If a type mismatch occurred, a warning is printed an NULL is returned. If the setting does not exist, NULL is returned. 
     */
    template <typename T, SettingType type> T* getSetting(const String& name) noexcept;

    /**
     * @brief save all settings
     */
    void _saveSettings() noexcept;

    //store the settings of the app
    std::unordered_map<String, Setting> m_settings;

};

#else //else, create an opaque wrapper

/**
 * @brief store an opaque settings structure
 * 
 * DO NOT DEREFERENCE, THE UNUSED ELEMENT IS ONLY SO MSVC COMPILES THE CODE
 */
typedef struct s_Settings {uint8_t unused} Settings;

#endif

/**
 * @brief Create a new settings object
 * 
 * @return Settings* a pointer to the new settings object
 */
Settings* settings_Create();

/**
 * @brief delete a settings object
 * 
 * @param settings a pointer to the settings object to delete
 */
void settings_Delete(Settings* settings);

/**
 * @brief Save the settings stored in a settings object to a file
 * 
 * @param file the path of the file to store the settings to
 * @param settings a pointer to the settings object to save
 */
void settings_SaveToFile(const char* file, Settings* settings);

/**
 * @brief Load a settings object from a file
 * 
 * @param file the path to the file to load the settings from
 * @param settings a pointer to the settings object to load to
 */
void settings_LoadFromFile(const char* file, Settings* settings);

/**
 * @brief get the amount of setting objects available
 * 
 * @param settings the setting object to quarry the information from
 * @return uint64_t the amount of settings the settings object has
 */
uint64_t settings_GetSettingCount(const Settings* settings);

/**
 * @brief Get the name of a setting that is quarried by the index
 * 
 * @param index the index to quarry the setting's name from
 * @param settings the settings object to quarry the information from
 * @return const String* a pointer to the setting's name
 */
const String* settings_getSettingName(uint64_t index, const Settings* settings);

/**
 * @brief get a setting by an index
 * 
 * @param index the index to quarry the setting from
 * @param settings a pointer to the settings object to quarry the information from
 * @return const Setting* a pointer to the setting element
 */
const Setting* settings_getSetting(uint64_t index, const Settings* settings);

//integers

/**
 * @brief Set a single setting element to a signed 8 bit integer
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_Int8(const String* name, int8_t value, Settings* settings);

/**
 * @brief Set a single setting element to an unsigned 8 bit integer
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_UInt8(const String* name, uint8_t value, Settings* settings);

/**
 * @brief Set a single setting element to a signed 16 bit integer
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_Int16(const String* name, int16_t value, Settings* settings);

/**
 * @brief Set a single setting element to an unsigned 16 bit integer
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_UInt16(const String* name, uint16_t value, Settings* settings);

/**
 * @brief Set a single setting element to a signed 32 bit integer
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_Int32(const String* name, int32_t value, Settings* settings);

/**
 * @brief Set a single setting element to an unsigned 32 bit integer
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_UInt32(const String* name, uint32_t value, Settings* settings);

/**
 * @brief Set a single setting element to a signed 64 bit integer
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_Int64(const String* name, int64_t value, Settings* settings);

/**
 * @brief Set a single setting element to an unsigned 64 bit integer
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_UInt64(const String* name, uint64_t value, Settings* settings);

//floats

/**
 * @brief Set a single setting element to a 32 bit IEEE float
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_Float(const String* name, float value, Settings* settings);

/**
 * @brief Set a single setting element to 64 bit IEEE float (double)
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_Double(const String* name, double value, Settings* settings);

//strings

/**
 * @brief Set a single setting element to a string
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_String(const String* name, const String* value, Settings* settings);

//float vectors

/**
 * @brief Set a single setting element to a 2 element vector of floats
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_Vec2(const String* name, const vec2* value, Settings* settings);

/**
 * @brief Set a single setting element to a 3 element vector of floats
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_Vec3(const String* name, const vec3* value, Settings* settings);

/**
 * @brief Set a single setting element to a 4 element vector of floats
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_Vec4(const String* name, const vec4* value, Settings* settings);

//double vectors

/**
 * @brief Set a single setting element to a 2 element vector of doubles
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_DVec2(const String* name, const dvec2* value, Settings* settings);

/**
 * @brief Set a single setting element to a 3 element vector of doubles
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_DVec3(const String* name, const dvec3* value, Settings* settings);

/**
 * @brief Set a single setting element to a 4 element vector of doubles
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_DVec4(const String* name, const dvec4* value, Settings* settings);

//uint32_t vectors

/**
 * @brief Set a single setting element to a 2 element vector of unsigned 32 bit integers
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_UIVec2(const String* name, const uivec2* value, Settings* settings);

/**
 * @brief Set a single setting element to a 3 element vector of unsigned 32 bit integers
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_UIVec3(const String* name, const uivec3* value, Settings* settings);

/**
 * @brief Set a single setting element to a 4 element vector of unsigned 32 bit integers
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_UIVec4(const String* name, const uivec4* value, Settings* settings);

//int32_t vectors

/**
 * @brief Set a single setting element to a 2 element vector of signed 32 bit integers
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_IVec2(const String* name, const ivec2* value, Settings* settings);

/**
 * @brief Set a single setting element to a 3 element vector of signed 32 bit integers
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_IVec3(const String* name, const ivec3* value, Settings* settings);

/**
 * @brief Set a single setting element to a 4 element vector of signed 32 bit integers
 * 
 * @param name the name of the setting to change / initialize
 * @param value the new value for the value
 * @param settings a pointer to the settings object to set the setting for
 */
void settings_SetSetting_IVec4(const String* name, const ivec4* value, Settings* settings);

//integers

/**
 * @brief Get the state of a setting with the type of an 8 bit signed integer
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
int8_t* settings_GetSetting_Int8(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of an 8 bit unsigned integer
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
uint8_t* settings_GetSetting_UInt8(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of an 16 bit signed integer
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
int16_t* settings_GetSetting_Int16(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of an 16 bit unsigned integer
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
uint16_t* settings_GetSetting_UInt16(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of an 32 bit signed integer
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
int32_t* settings_GetSetting_Int32(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of an 32 bit unsigned integer
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
uint32_t* settings_GetSetting_UInt32(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of an 64 bit signed integer
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
int64_t* settings_GetSetting_Int64(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of an 64 bit unsigned integer
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
uint64_t* settings_GetSetting_UInt64(const String* name, Settings* settings);

//floats

/**
 * @brief Get the state of a setting with the type of a 32 bit IEEE float
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
float* settings_GetSetting_Float(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of a 64 bit IEEE float (double)
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
double* settings_GetSetting_Double(const String* name, Settings* settings);

//strings

/**
 * @brief Get the state of a setting with the type of a string
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
String* settings_GetSetting_String(const String* name, Settings* settings);

//float vectors

/**
 * @brief Get the state of a setting with the type of a float vector with 2 elements
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
vec2* settings_GetSetting_Vec2(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of a float vector with 3 elements
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
vec3* settings_GetSetting_Vec3(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of a float vector with 4 elements
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
vec4* settings_GetSetting_Vec4(const String* name, Settings* settings);

//double vectors

/**
 * @brief Get the state of a setting with the type of a double vector with 2 elements
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
dvec2* settings_GetSetting_DVec2(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of a double vector with 3 elements
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
dvec3* settings_GetSetting_DVec3(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of a double vector with 4 elements
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
dvec4* settings_GetSetting_DVec4(const String* name, Settings* settings);

//uint32_t vectors

/**
 * @brief Get the state of a setting with the type of a unsigned 32 bit integer vector with 2 elements
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
uivec2* settings_GetSetting_UIVec2(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of a unsigned 32 bit integer vector with 3 elements
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
uivec3* settings_GetSetting_UIVec3(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of a unsigned 32 bit integer vector with 4 elements
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
uivec4* settings_GetSetting_UIVec4(const String* name, Settings* settings);

//int32_t vectors

/**
 * @brief Get the state of a setting with the type of a signed 32 bit integer vector with 2 elements
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
ivec2* settings_GetSetting_IVec2(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of a signed 32 bit integer vector with 3 elements
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
ivec3* settings_GetSetting_IVec3(const String* name, Settings* settings);

/**
 * @brief Get the state of a setting with the type of a signed 32 bit integer vector with 4 elements
 * 
 * @param name the name of the setting to quarry
 * @param settings a pointer to the settings object to quarry the element from
 * @return a pointer to the value or NULL if something went wrong (the element was not found / type mismatch)
 */
ivec4* settings_GetSetting_IVec4(const String* name, Settings* settings);

#endif