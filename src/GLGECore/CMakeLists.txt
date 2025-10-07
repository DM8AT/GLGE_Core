cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0111 OLD)

project(GLGE_CORE_PROJ LANGUAGES CXX)

set(GLGE_CORE_VERSION 0.1.0)

# ------------------------------
# Begin GLGE_CORE library setup
# ------------------------------

set(GLGE_CORE_SRC 
    Messages/MessageListener.cpp

    Events/EventHandler.cpp
    Events/Event.cpp

    Layers/LayerStack.cpp

    Application/App.cpp

    Setting/Settings.cpp

    Filesystem/File.cpp
    Filesystem/Compression.cpp
    Filesystem/Encryption.cpp
)

add_library(GLGE_CORE ${GLGE_CORE_SRC})

# enable avx512. Required for Float16 on MSVC 
target_compile_options(GLGE_CORE PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX512FP16>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-mavx512fp16>
)

# Compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow -Wdouble-promotion -Wformat=2 -Wnull-dereference -Wundef -Wcast-align -Wmissing-declarations -Wunreachable-code -Winit-self -Wlogical-op")

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# MSVC runtime
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

# C++20 and versioning
set_target_properties(GLGE_CORE PROPERTIES 
    CXX_STANDARD 20 
    CXX_STANDARD_REQUIRED ON 
    VERSION ${GLGE_CORE_VERSION} 
    SOVERSION 1
)

# ------------------------------
# Other dependencies
# ------------------------------

## GLGE_BG
add_subdirectory(${PROJECT_SOURCE_DIR}/../GLGE_BG ${PROJECT_BINARY_DIR}/../GLGE_BG)
target_link_libraries(GLGE_CORE PUBLIC GLGE_BG)

## PugiXML
set(CMAKE_CXX_FLAGS_DEBUG "-O2")
add_subdirectory(${PROJECT_SOURCE_DIR}/../external/pugixml ${PROJECT_BINARY_DIR}/../external/pugixml)
target_link_libraries(GLGE_CORE PRIVATE pugixml)
target_include_directories(GLGE_CORE PRIVATE ${PROJECT_SOURCE_DIR}/../external/pugixml/src)

## Zlib
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/../external/zlib ${PROJECT_BINARY_DIR}/../external/zlib)
target_link_libraries(GLGE_CORE PRIVATE zlibstatic)
target_link_directories(GLGE_CORE PRIVATE ${PROJECT_SOURCE_DIR}/../external/zlib)
## Add OpenSSL
find_package(OpenSSL QUIET)
if (OpenSSL_FOUND)
    # Add OpenSSL
    message(STATUS "System OpenSSL found: ${OpenSSL_VERSION}")
    target_link_libraries(GLGE_CORE PRIVATE OpenSSL::SSL OpenSSL::Crypto)
else()
    # Fatal error - OpenSSL is required
    message(FATAL_ERROR "OpenSSL not found. Please install it via a package manager (vcpkg / apt / brew / choco / conan).")
endif()
