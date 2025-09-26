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

//include pugixml for XML
#include "../../external/pugixml/src/pugixml.hpp"
//include the setting API
#include "Settings.h"
//include GLGE error handling
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

//include the C++ filesystem
#include <filesystem>

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

//encode function specialization
//integer types
template <> void SettingValue::encode<SETTING_TYPE_INT8>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_INT8);
    node.append_attribute("value").set_value(_int8_t);
}
template <> void SettingValue::encode<SETTING_TYPE_UINT8>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_UINT8);
    node.append_attribute("value").set_value(_uint8_t);
}
template <> void SettingValue::encode<SETTING_TYPE_INT16>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_INT16);
    node.append_attribute("value").set_value(_int16_t);
}
template <> void SettingValue::encode<SETTING_TYPE_UINT16>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_UINT16);
    node.append_attribute("value").set_value(_uint16_t);
}
template <> void SettingValue::encode<SETTING_TYPE_INT32>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_INT32);
    node.append_attribute("value").set_value(_int32_t);
}
template <> void SettingValue::encode<SETTING_TYPE_UINT32>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_UINT32);
    node.append_attribute("value").set_value(_uint32_t);
}
template <> void SettingValue::encode<SETTING_TYPE_INT64>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_INT64);
    node.append_attribute("value").set_value(_int64_t);
}
template <> void SettingValue::encode<SETTING_TYPE_UINT64>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_UINT64);
    node.append_attribute("value").set_value(_uint64_t);
}
//float types
template <> void SettingValue::encode<SETTING_TYPE_FLOAT>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_FLOAT);
    node.append_attribute("value").set_value(_float);
}
template <> void SettingValue::encode<SETTING_TYPE_DOUBLE>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_DOUBLE);
    node.append_attribute("value").set_value(_double);
}
//string type
template <> void SettingValue::encode<SETTING_TYPE_STRING>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_STRING);
    node.append_attribute("value").set_value(_string.c_str());
}
//float vectors
template <> void SettingValue::encode<SETTING_TYPE_VEC2>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_VEC2);
    node.append_attribute("x").set_value(_vec2.x);
    node.append_attribute("y").set_value(_vec2.y);
}
template <> void SettingValue::encode<SETTING_TYPE_VEC3>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_VEC3);
    node.append_attribute("x").set_value(_vec3.x);
    node.append_attribute("y").set_value(_vec3.y);
    node.append_attribute("z").set_value(_vec3.z);
}
template <> void SettingValue::encode<SETTING_TYPE_VEC4>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_VEC4);
    node.append_attribute("x").set_value(_vec4.x);
    node.append_attribute("y").set_value(_vec4.y);
    node.append_attribute("z").set_value(_vec4.z);
    node.append_attribute("w").set_value(_vec4.w);
}
//double vectors
template <> void SettingValue::encode<SETTING_TYPE_DVEC2>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_DVEC2);
    node.append_attribute("x").set_value(_dvec2.x);
    node.append_attribute("y").set_value(_dvec2.y);
}
template <> void SettingValue::encode<SETTING_TYPE_DVEC3>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_DVEC3);
    node.append_attribute("x").set_value(_dvec3.x);
    node.append_attribute("y").set_value(_dvec3.y);
    node.append_attribute("z").set_value(_dvec3.z);
}
template <> void SettingValue::encode<SETTING_TYPE_DVEC4>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_DVEC4);
    node.append_attribute("x").set_value(_dvec4.x);
    node.append_attribute("y").set_value(_dvec4.y);
    node.append_attribute("z").set_value(_dvec4.z);
    node.append_attribute("w").set_value(_dvec4.w);
}
//uint32_t vectors
template <> void SettingValue::encode<SETTING_TYPE_UIVEC2>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_UIVEC2);
    node.append_attribute("x").set_value(_uivec2.x);
    node.append_attribute("y").set_value(_uivec2.y);
}
template <> void SettingValue::encode<SETTING_TYPE_UIVEC3>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_UIVEC3);
    node.append_attribute("x").set_value(_uivec3.x);
    node.append_attribute("y").set_value(_uivec3.y);
    node.append_attribute("z").set_value(_uivec3.z);
}
template <> void SettingValue::encode<SETTING_TYPE_UIVEC4>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_UIVEC4);
    node.append_attribute("x").set_value(_uivec4.x);
    node.append_attribute("y").set_value(_uivec4.y);
    node.append_attribute("z").set_value(_uivec4.z);
    node.append_attribute("w").set_value(_uivec4.w);
}
//int32_t vectors
template <> void SettingValue::encode<SETTING_TYPE_IVEC2>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_IVEC2);
    node.append_attribute("x").set_value(_ivec2.x);
    node.append_attribute("y").set_value(_ivec2.y);
}
template <> void SettingValue::encode<SETTING_TYPE_IVEC3>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_IVEC3);
    node.append_attribute("x").set_value(_ivec3.x);
    node.append_attribute("y").set_value(_ivec3.y);
    node.append_attribute("z").set_value(_ivec3.z);
}
template <> void SettingValue::encode<SETTING_TYPE_IVEC4>(pugi::xml_node& parent) const
{
    pugi::xml_node node = parent.append_child("Value");
    node.append_attribute("type").set_value((uint64_t)SETTING_TYPE_IVEC4);
    node.append_attribute("x").set_value(_ivec4.x);
    node.append_attribute("y").set_value(_ivec4.y);
    node.append_attribute("z").set_value(_ivec4.z);
    node.append_attribute("w").set_value(_ivec4.w);
}

//decode implementations
//integer types
template <> void SettingValue::__decodeType<SETTING_TYPE_INT8>(pugi::xml_node& value)
{_int8_t = (int8_t)value.attribute("value").as_int();}
template <> void SettingValue::__decodeType<SETTING_TYPE_UINT8>(pugi::xml_node& value)
{_uint8_t = (uint8_t)value.attribute("value").as_uint();}
template <> void SettingValue::__decodeType<SETTING_TYPE_INT16>(pugi::xml_node& value)
{_int16_t = (int16_t)value.attribute("value").as_int();}
template <> void SettingValue::__decodeType<SETTING_TYPE_UINT16>(pugi::xml_node& value)
{_uint16_t = (uint16_t)value.attribute("value").as_uint();}
template <> void SettingValue::__decodeType<SETTING_TYPE_INT32>(pugi::xml_node& value)
{_int32_t = (int32_t)value.attribute("value").as_int();}
template <> void SettingValue::__decodeType<SETTING_TYPE_UINT32>(pugi::xml_node& value)
{_uint32_t = (uint32_t)value.attribute("value").as_uint();}
template <> void SettingValue::__decodeType<SETTING_TYPE_INT64>(pugi::xml_node& value)
{_int64_t = (int64_t)value.attribute("value").as_llong();}
template <> void SettingValue::__decodeType<SETTING_TYPE_UINT64>(pugi::xml_node& value)
{_uint64_t = (uint64_t)value.attribute("value").as_ullong();}
//float types
template <> void SettingValue::__decodeType<SETTING_TYPE_FLOAT>(pugi::xml_node& value)
{_float = value.attribute("value").as_float();}
template <> void SettingValue::__decodeType<SETTING_TYPE_DOUBLE>(pugi::xml_node& value)
{_double = value.attribute("value").as_double();}
//string
template <> void SettingValue::__decodeType<SETTING_TYPE_STRING>(pugi::xml_node& value)
{_string = value.attribute("value").as_string();}
//float vector
template <> void SettingValue::__decodeType<SETTING_TYPE_VEC2>(pugi::xml_node& value)
{_vec2.x = value.attribute("x").as_float();
 _vec2.y = value.attribute("y").as_float();}
template <> void SettingValue::__decodeType<SETTING_TYPE_VEC3>(pugi::xml_node& value)
{_vec3.x = value.attribute("x").as_float();
 _vec3.y = value.attribute("y").as_float();
 _vec3.z = value.attribute("z").as_float();}
template <> void SettingValue::__decodeType<SETTING_TYPE_VEC4>(pugi::xml_node& value)
{_vec4.x = value.attribute("x").as_float();
 _vec4.y = value.attribute("y").as_float();
 _vec4.z = value.attribute("z").as_float();
 _vec4.w = value.attribute("w").as_float();}
//double vector
template <> void SettingValue::__decodeType<SETTING_TYPE_DVEC2>(pugi::xml_node& value)
{_dvec2.x = value.attribute("x").as_double();
 _dvec2.y = value.attribute("y").as_double();}
template <> void SettingValue::__decodeType<SETTING_TYPE_DVEC3>(pugi::xml_node& value)
{_dvec3.x = value.attribute("x").as_double();
 _dvec3.y = value.attribute("y").as_double();
 _dvec3.z = value.attribute("z").as_double();}
template <> void SettingValue::__decodeType<SETTING_TYPE_DVEC4>(pugi::xml_node& value)
{_dvec4.x = value.attribute("x").as_double();
 _dvec4.y = value.attribute("y").as_double();
 _dvec4.z = value.attribute("z").as_double();
 _dvec4.w = value.attribute("w").as_double();}
//uint32_t vector
template <> void SettingValue::__decodeType<SETTING_TYPE_UIVEC2>(pugi::xml_node& value)
{_uivec2.x = value.attribute("x").as_uint();
 _uivec2.y = value.attribute("y").as_uint();}
template <> void SettingValue::__decodeType<SETTING_TYPE_UIVEC3>(pugi::xml_node& value)
{_uivec3.x = value.attribute("x").as_uint();
 _uivec3.y = value.attribute("y").as_uint();
 _uivec3.z = value.attribute("z").as_uint();}
template <> void SettingValue::__decodeType<SETTING_TYPE_UIVEC4>(pugi::xml_node& value)
{_uivec4.x = value.attribute("x").as_uint();
 _uivec4.y = value.attribute("y").as_uint();
 _uivec4.z = value.attribute("z").as_uint();
 _uivec4.w = value.attribute("w").as_uint();}
//int32_t vector
template <> void SettingValue::__decodeType<SETTING_TYPE_IVEC2>(pugi::xml_node& value)
{_ivec2.x = value.attribute("x").as_int();
 _ivec2.y = value.attribute("y").as_int();}
template <> void SettingValue::__decodeType<SETTING_TYPE_IVEC3>(pugi::xml_node& value)
{_ivec3.x = value.attribute("x").as_int();
 _ivec3.y = value.attribute("y").as_int();
 _ivec3.z = value.attribute("z").as_int();}
template <> void SettingValue::__decodeType<SETTING_TYPE_IVEC4>(pugi::xml_node& value)
{_ivec4.x = value.attribute("x").as_int();
 _ivec4.y = value.attribute("y").as_int();
 _ivec4.z = value.attribute("z").as_int();
 _ivec4.w = value.attribute("w").as_int();}

//define a helper for the switch to decode the correct types
#define __GLGE_SETTING_HELPER(type) \
case type: \
    __decodeType<type>(value); \
    break;

SettingType SettingValue::decode(pugi::xml_node& value, uint64_t type)
{
    //decode the type
    SettingType _type = (SettingType)type;

    //decode the value in the correct way
    switch (_type)
    {
    //integers
    __GLGE_SETTING_HELPER(SETTING_TYPE_INT8)
    __GLGE_SETTING_HELPER(SETTING_TYPE_UINT8)
    __GLGE_SETTING_HELPER(SETTING_TYPE_INT16)
    __GLGE_SETTING_HELPER(SETTING_TYPE_UINT16)
    __GLGE_SETTING_HELPER(SETTING_TYPE_INT32)
    __GLGE_SETTING_HELPER(SETTING_TYPE_UINT32)
    __GLGE_SETTING_HELPER(SETTING_TYPE_INT64)
    __GLGE_SETTING_HELPER(SETTING_TYPE_UINT64)
    //floats
    __GLGE_SETTING_HELPER(SETTING_TYPE_FLOAT)
    __GLGE_SETTING_HELPER(SETTING_TYPE_DOUBLE)
    //strings
    __GLGE_SETTING_HELPER(SETTING_TYPE_STRING)
    //float vectors
    __GLGE_SETTING_HELPER(SETTING_TYPE_VEC2)
    __GLGE_SETTING_HELPER(SETTING_TYPE_VEC3)
    __GLGE_SETTING_HELPER(SETTING_TYPE_VEC4)
    //double vectors
    __GLGE_SETTING_HELPER(SETTING_TYPE_DVEC2)
    __GLGE_SETTING_HELPER(SETTING_TYPE_DVEC3)
    __GLGE_SETTING_HELPER(SETTING_TYPE_DVEC4)
    //uint32_t vectors
    __GLGE_SETTING_HELPER(SETTING_TYPE_UIVEC2)
    __GLGE_SETTING_HELPER(SETTING_TYPE_UIVEC3)
    __GLGE_SETTING_HELPER(SETTING_TYPE_UIVEC4)
    //int32_t vectors
    __GLGE_SETTING_HELPER(SETTING_TYPE_IVEC2)
    __GLGE_SETTING_HELPER(SETTING_TYPE_IVEC3)
    __GLGE_SETTING_HELPER(SETTING_TYPE_IVEC4)
    
    default:
        break;
    }

    //return the type
    return _type;
}

//delete the helper
#undef __GLGE_SETTING_HELPER


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

#define __GLGE_SETTING_HELPER(type)\
case type: \
    setting.value.encode<type>(set); \
    break;

/**
 * @brief add a single setting element to the document
 * 
 * @param node a reference to the node to attach the elements to
 * @param name the name of the setting to store
 * @param setting the actual setting to store
 */
static void __addSettingToDoc(pugi::xml_node& node, const String& name, const Setting& setting)
{
    //add the general setting element
    pugi::xml_node set = node.append_child("Setting");
    //store the name
    set.append_attribute("name").set_value(name.c_str());
    //store the type's value
    switch (setting.type)
    {
    //integers
    __GLGE_SETTING_HELPER(SETTING_TYPE_INT8)
    __GLGE_SETTING_HELPER(SETTING_TYPE_UINT8)
    __GLGE_SETTING_HELPER(SETTING_TYPE_INT16)
    __GLGE_SETTING_HELPER(SETTING_TYPE_UINT16)
    __GLGE_SETTING_HELPER(SETTING_TYPE_INT32)
    __GLGE_SETTING_HELPER(SETTING_TYPE_UINT32)
    __GLGE_SETTING_HELPER(SETTING_TYPE_INT64)
    __GLGE_SETTING_HELPER(SETTING_TYPE_UINT64)
    //float
    __GLGE_SETTING_HELPER(SETTING_TYPE_FLOAT)
    __GLGE_SETTING_HELPER(SETTING_TYPE_DOUBLE)
    //strings
    __GLGE_SETTING_HELPER(SETTING_TYPE_STRING)
    //float vectors
    __GLGE_SETTING_HELPER(SETTING_TYPE_VEC2)
    __GLGE_SETTING_HELPER(SETTING_TYPE_VEC3)
    __GLGE_SETTING_HELPER(SETTING_TYPE_VEC4)
    //double vectors
    __GLGE_SETTING_HELPER(SETTING_TYPE_DVEC2)
    __GLGE_SETTING_HELPER(SETTING_TYPE_DVEC3)
    __GLGE_SETTING_HELPER(SETTING_TYPE_DVEC4)
    //uint32_t vectors
    __GLGE_SETTING_HELPER(SETTING_TYPE_UIVEC2)
    __GLGE_SETTING_HELPER(SETTING_TYPE_UIVEC3)
    __GLGE_SETTING_HELPER(SETTING_TYPE_UIVEC4)
    //int32_t vectors
    __GLGE_SETTING_HELPER(SETTING_TYPE_IVEC2)
    __GLGE_SETTING_HELPER(SETTING_TYPE_IVEC3)
    __GLGE_SETTING_HELPER(SETTING_TYPE_IVEC4)
    
    default:
        break;
    }
}

//delete the helper
#undef __GLGE_SETTING_HELPER

void Settings::saveToFile(const char* file)
{
    //create the pugixml file to save the settings to
    pugi::xml_document doc;
    pugi::xml_node settings = doc.append_child("Settings");
    //iterate over all settings to save them
    for (auto it = m_settings.begin(); it != m_settings.end(); ++it)
    {__addSettingToDoc(settings, it->first, it->second);}

    //save the pugixml file
    GLGE_ASSERT("Failed to save the file " << std::filesystem::path(file), !doc.save_file(std::filesystem::path(file).c_str()));
}

void Settings::loadFromFile(const char* file)
{
    //clear the settings
    m_settings.clear();
    //load the file
    pugi::xml_document doc;
    pugi::xml_parse_result res = doc.load_file(std::filesystem::path(file).c_str());

    //error checking
    if (!res)
    {
        std::cerr << "[ERROR] Failed to load file " << std::filesystem::path(file) << " :\n" << res.description() << "\n";
        return;
    }

    //load the actual stuff
    pugi::xml_node setts = doc.child("Settings");
    if (!setts) {
        std::cerr << "[ERROR] Failed to parse file " << std::filesystem::path(file) << ": No element named \"Settings\" exists\n";
        return;
    }

    //iterate over all children
    for (pugi::xml_node_iterator it = setts.begin(); it != setts.end(); ++it)
    {
        //load the node
        pugi::xml_attribute _name = it->attribute("name");

        //create the actual setting
        pugi::xml_node val = it->child("Value");
        m_settings[_name.as_string()].type = m_settings[_name.as_string()].value.decode(val, val.attribute("type").as_ullong());
    }
}

//specialize the print function
//integers
template <> std::ostream& Setting::printSelf<SETTING_TYPE_INT8>(std::ostream& os) const {return os << value._int8_t;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_UINT8>(std::ostream& os) const {return os << value._uint8_t;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_INT16>(std::ostream& os) const {return os << value._int16_t;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_UINT16>(std::ostream& os) const {return os << value._uint16_t;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_INT32>(std::ostream& os) const {return os << value._int32_t;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_UINT32>(std::ostream& os) const {return os << value._uint32_t;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_INT64>(std::ostream& os) const {return os << value._int64_t;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_UINT64>(std::ostream& os) const {return os << value._uint64_t;}
//floats
template <> std::ostream& Setting::printSelf<SETTING_TYPE_FLOAT>(std::ostream& os) const {return os << value._float;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_DOUBLE>(std::ostream& os) const {return os << value._double;}
//string
template <> std::ostream& Setting::printSelf<SETTING_TYPE_STRING>(std::ostream& os) const {return os << value._string;}
//float vectors
template <> std::ostream& Setting::printSelf<SETTING_TYPE_VEC2>(std::ostream& os) const {return os << value._vec2;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_VEC3>(std::ostream& os) const {return os << value._vec3;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_VEC4>(std::ostream& os) const {return os << value._vec4;}
//double vectors
template <> std::ostream& Setting::printSelf<SETTING_TYPE_DVEC2>(std::ostream& os) const {return os << value._dvec2;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_DVEC3>(std::ostream& os) const {return os << value._dvec3;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_DVEC4>(std::ostream& os) const {return os << value._dvec4;}
//uint32_t vectors
template <> std::ostream& Setting::printSelf<SETTING_TYPE_UIVEC2>(std::ostream& os) const {return os << value._uivec2;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_UIVEC3>(std::ostream& os) const {return os << value._uivec3;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_UIVEC4>(std::ostream& os) const {return os << value._uivec4;}
//int32_t vectors
template <> std::ostream& Setting::printSelf<SETTING_TYPE_IVEC2>(std::ostream& os) const {return os << value._ivec2;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_IVEC3>(std::ostream& os) const {return os << value._ivec3;}
template <> std::ostream& Setting::printSelf<SETTING_TYPE_IVEC4>(std::ostream& os) const {return os << value._ivec4;}

//a helper define to use for setting switches
#define __GLGE_HELPER_SETTING(type) \
case type: \
    return setting.printSelf<type>(os);

std::ostream& operator<<(std::ostream& os, const s_Setting& setting) noexcept
{
    switch (setting.type)
    {
    //integers
    __GLGE_HELPER_SETTING(SETTING_TYPE_INT8)
    __GLGE_HELPER_SETTING(SETTING_TYPE_UINT8)
    __GLGE_HELPER_SETTING(SETTING_TYPE_INT16)
    __GLGE_HELPER_SETTING(SETTING_TYPE_UINT16)
    __GLGE_HELPER_SETTING(SETTING_TYPE_INT32)
    __GLGE_HELPER_SETTING(SETTING_TYPE_UINT32)
    __GLGE_HELPER_SETTING(SETTING_TYPE_INT64)
    __GLGE_HELPER_SETTING(SETTING_TYPE_UINT64)
    //floats
    __GLGE_HELPER_SETTING(SETTING_TYPE_FLOAT)
    __GLGE_HELPER_SETTING(SETTING_TYPE_DOUBLE)
    //string
    __GLGE_HELPER_SETTING(SETTING_TYPE_STRING)
    //float vectors
    __GLGE_HELPER_SETTING(SETTING_TYPE_VEC2)
    __GLGE_HELPER_SETTING(SETTING_TYPE_VEC3)
    __GLGE_HELPER_SETTING(SETTING_TYPE_VEC4)
    //doubles vectors
    __GLGE_HELPER_SETTING(SETTING_TYPE_DVEC2)
    __GLGE_HELPER_SETTING(SETTING_TYPE_DVEC3)
    __GLGE_HELPER_SETTING(SETTING_TYPE_DVEC4)
    //uint32_t vectors
    __GLGE_HELPER_SETTING(SETTING_TYPE_IVEC2)
    __GLGE_HELPER_SETTING(SETTING_TYPE_IVEC3)
    __GLGE_HELPER_SETTING(SETTING_TYPE_IVEC4)
    //int32_t vectors
    __GLGE_HELPER_SETTING(SETTING_TYPE_UIVEC2)
    __GLGE_HELPER_SETTING(SETTING_TYPE_UIVEC3)
    __GLGE_HELPER_SETTING(SETTING_TYPE_UIVEC4)
    
    default:
        break;
    }

    //just print nothin'
    return os;
}

//delete the helper define
#undef __GLGE_HELPER_SETTING
