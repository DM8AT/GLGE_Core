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
    inline uint8_t* getSetting_Unt8(const String& name) noexcept {return getSetting<uint8_t, SETTING_TYPE_UINT8>(name);}

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
    inline uint16_t* getSetting_Unt16(const String& name) noexcept {return getSetting<uint16_t, SETTING_TYPE_UINT16>(name);}

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
    inline uint32_t* getSetting_Unt32(const String& name) noexcept {return getSetting<uint32_t, SETTING_TYPE_UINT32>(name);}

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
    inline uint64_t* getSetting_Unt64(const String& name) noexcept {return getSetting<uint64_t, SETTING_TYPE_UINT64>(name);}

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

#endif

#endif