/**
 * @file Settings.cpp
 * @author DM8AT
 * @brief implement the setting interface and the binding functions
 * @version 0.1
 * @date 2025-09-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the setting API
#include "Settings.h"

//template implementations for the = operator
//uint
template <> void SettingValue::operator=(const uint8_t& v) noexcept {_uint8_t = v;}
template <> void SettingValue::operator=(const uint16_t& v) noexcept {_uint16_t = v;}
template <> void SettingValue::operator=(const uint32_t& v) noexcept {_uint32_t = v;}
template <> void SettingValue::operator=(const uint64_t& v) noexcept {_uint64_t = v;}
//int
template <> void SettingValue::operator=(const int8_t& v) noexcept {_int8_t = v;}
template <> void SettingValue::operator=(const int16_t& v) noexcept {_int16_t = v;}
template <> void SettingValue::operator=(const int32_t& v) noexcept {_int32_t = v;}
template <> void SettingValue::operator=(const int64_t& v) noexcept {_int64_t = v;}
//float
template <> void SettingValue::operator=(const float& v) noexcept {_float = v;}
template <> void SettingValue::operator=(const double& v) noexcept {_double = v;}
//string
template <> void SettingValue::operator=(const String& v) noexcept {_string = v;}
//float vectors
template <> void SettingValue::operator=(const vec2& v) noexcept {_vec2 = v;}
template <> void SettingValue::operator=(const vec3& v) noexcept {_vec3 = v;}
template <> void SettingValue::operator=(const vec4& v) noexcept {_vec4 = v;}
//double vectors
template <> void SettingValue::operator=(const dvec2& v) noexcept {_dvec2 = v;}
template <> void SettingValue::operator=(const dvec3& v) noexcept {_dvec3 = v;}
template <> void SettingValue::operator=(const dvec4& v) noexcept {_dvec4 = v;}
//uint vectors
template <> void SettingValue::operator=(const uivec2& v) noexcept {_uivec2 = v;}
template <> void SettingValue::operator=(const uivec3& v) noexcept {_uivec3 = v;}
template <> void SettingValue::operator=(const uivec4& v) noexcept {_uivec4 = v;}
//int vectors
template <> void SettingValue::operator=(const ivec2& v) noexcept {_ivec2 = v;}
template <> void SettingValue::operator=(const ivec3& v) noexcept {_ivec3 = v;}
template <> void SettingValue::operator=(const ivec4& v) noexcept {_ivec4 = v;}

//store the names of the app settings as strings
static constexpr const char* __SETTING_TYPE_NAMES[] = {
    "SETTING_TYPE_UINT8",
    "SETTING_TYPE_INT8",
    "SETTING_TYPE_UINT16",
    "SETTING_TYPE_INT16",
    "SETTING_TYPE_UINT32",
    "SETTING_TYPE_INT32",
    "SETTING_TYPE_UINT64",
    "SETTING_TYPE_INT64",
    "SETTING_TYPE_FLOAT",
    "SETTING_TYPE_DOUBLE",
    "SETTING_TYPE_STRING",
    "SETTING_TYPE_VEC2",
    "SETTING_TYPE_VEC3",
    "SETTING_TYPE_VEC4",
    "SETTING_TYPE_DVEC2",
    "SETTING_TYPE_DVEC3",
    "SETTING_TYPE_DVEC4",
    "SETTING_TYPE_UIVEC2",
    "SETTING_TYPE_UIVEC3",
    "SETTING_TYPE_UIVEC4",
    "SETTING_TYPE_IVEC2",
    "SETTING_TYPE_IVEC3",
    "SETTING_TYPE_IVEC4"
};

/**
 * @brief get the name of a app setting type
 * 
 * @param type the type to quarry the information for
 * @return constexpr const char const* the constant name of the type
 */
static constexpr const char* __getAppSettingName(const SettingType& type) noexcept {
    //return the corresponding type name
    return __SETTING_TYPE_NAMES[(uint64_t)type];
}

//general template implementation
template <typename T, SettingType type> void Settings::setSetting(const String& name, const T& value) noexcept
{
    //store the setting
    m_settings[name].type = type;
    m_settings[name].value = value;
}

//explicit template implementation
//integers
template void Settings::setSetting<uint8_t, SETTING_TYPE_UINT8>(const String& name, const uint8_t& value) noexcept;
template void Settings::setSetting<int8_t, SETTING_TYPE_INT8>(const String& name, const int8_t& value) noexcept;
template void Settings::setSetting<uint16_t, SETTING_TYPE_UINT16>(const String& name, const uint16_t& value) noexcept;
template void Settings::setSetting<int16_t, SETTING_TYPE_INT16>(const String& name, const int16_t& value) noexcept;
template void Settings::setSetting<uint32_t, SETTING_TYPE_UINT32>(const String& name, const uint32_t& value) noexcept;
template void Settings::setSetting<int32_t, SETTING_TYPE_INT32>(const String& name, const int32_t& value) noexcept;
template void Settings::setSetting<uint64_t, SETTING_TYPE_UINT64>(const String& name, const uint64_t& value) noexcept;
template void Settings::setSetting<int64_t, SETTING_TYPE_INT64>(const String& name, const int64_t& value) noexcept;
//floats
template void Settings::setSetting<float, SETTING_TYPE_FLOAT>(const String& name, const float& value) noexcept;
template void Settings::setSetting<double, SETTING_TYPE_DOUBLE>(const String& name, const double& value) noexcept;
//strings
template void Settings::setSetting<String, SETTING_TYPE_STRING>(const String& name, const String& value) noexcept;
//float vectors
template void Settings::setSetting<vec2, SETTING_TYPE_VEC2>(const String& name, const vec2& value) noexcept;
template void Settings::setSetting<vec3, SETTING_TYPE_VEC3>(const String& name, const vec3& value) noexcept;
template void Settings::setSetting<vec4, SETTING_TYPE_VEC4>(const String& name, const vec4& value) noexcept;
//double vectors
template void Settings::setSetting<dvec2, SETTING_TYPE_DVEC2>(const String& name, const dvec2& value) noexcept;
template void Settings::setSetting<dvec3, SETTING_TYPE_DVEC3>(const String& name, const dvec3& value) noexcept;
template void Settings::setSetting<dvec4, SETTING_TYPE_DVEC4>(const String& name, const dvec4& value) noexcept;
//uint vectors
template void Settings::setSetting<uivec2, SETTING_TYPE_UIVEC2>(const String& name, const uivec2& value) noexcept;
template void Settings::setSetting<uivec3, SETTING_TYPE_UIVEC3>(const String& name, const uivec3& value) noexcept;
template void Settings::setSetting<uivec4, SETTING_TYPE_UIVEC4>(const String& name, const uivec4& value) noexcept;
//int vectors
template void Settings::setSetting<ivec2, SETTING_TYPE_IVEC2>(const String& name, const ivec2& value) noexcept;
template void Settings::setSetting<ivec3, SETTING_TYPE_IVEC3>(const String& name, const ivec3& value) noexcept;
template void Settings::setSetting<ivec4, SETTING_TYPE_IVEC4>(const String& name, const ivec4& value) noexcept;

//general template implementation
template <typename T, SettingType type> T* Settings::getSetting(const String& name) noexcept
{
    //quarry the element
    auto it = m_settings.find(name);
    //if it was not found, return NULL
    if (it == m_settings.end()) {return NULL;}
    //else, compare the type values to prevent a type mismatch
    if (it->second.type == type) {return (T*)&(it->second.value);}
    else {
        std::cerr << "[WARNING] Type mismatch for app setting \"" << name << "\": Setting is stored of type " << __getAppSettingName(it->second.type) << ", but was requested as type " << __getAppSettingName(type) << "\n";
        return NULL;
    }
}

//explicit template implementation
//integers
template uint8_t* Settings::getSetting<uint8_t, SETTING_TYPE_UINT8>(const String& name) noexcept;
template int8_t* Settings::getSetting<int8_t, SETTING_TYPE_INT8>(const String& name) noexcept;
template uint16_t* Settings::getSetting<uint16_t, SETTING_TYPE_UINT16>(const String& name) noexcept;
template int16_t* Settings::getSetting<int16_t, SETTING_TYPE_INT16>(const String& name) noexcept;
template uint32_t* Settings::getSetting<uint32_t, SETTING_TYPE_UINT32>(const String& name) noexcept;
template int32_t* Settings::getSetting<int32_t, SETTING_TYPE_INT32>(const String& name) noexcept;
template uint64_t* Settings::getSetting<uint64_t, SETTING_TYPE_UINT64>(const String& name) noexcept;
template int64_t* Settings::getSetting<int64_t, SETTING_TYPE_INT64>(const String& name) noexcept;
//floats
template float* Settings::getSetting<float, SETTING_TYPE_FLOAT>(const String& name) noexcept;
template double* Settings::getSetting<double, SETTING_TYPE_DOUBLE>(const String& name) noexcept;
//string
template String* Settings::getSetting<String, SETTING_TYPE_STRING>(const String& name) noexcept;
//float vectors
template vec2* Settings::getSetting<vec2, SETTING_TYPE_VEC2>(const String& name) noexcept;
template vec3* Settings::getSetting<vec3, SETTING_TYPE_VEC3>(const String& name) noexcept;
template vec4* Settings::getSetting<vec4, SETTING_TYPE_VEC4>(const String& name) noexcept;
//double vectors
template dvec2* Settings::getSetting<dvec2, SETTING_TYPE_DVEC2>(const String& name) noexcept;
template dvec3* Settings::getSetting<dvec3, SETTING_TYPE_DVEC3>(const String& name) noexcept;
template dvec4* Settings::getSetting<dvec4, SETTING_TYPE_DVEC4>(const String& name) noexcept;
//uint vectors
template uivec2* Settings::getSetting<uivec2, SETTING_TYPE_UIVEC2>(const String& name) noexcept;
template uivec3* Settings::getSetting<uivec3, SETTING_TYPE_UIVEC3>(const String& name) noexcept;
template uivec4* Settings::getSetting<uivec4, SETTING_TYPE_UIVEC4>(const String& name) noexcept;
//int vectors
template ivec2* Settings::getSetting<ivec2, SETTING_TYPE_IVEC2>(const String& name) noexcept;
template ivec3* Settings::getSetting<ivec3, SETTING_TYPE_IVEC3>(const String& name) noexcept;
template ivec4* Settings::getSetting<ivec4, SETTING_TYPE_IVEC4>(const String& name) noexcept;
