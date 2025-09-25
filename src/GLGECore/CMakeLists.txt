# set the minimum required version
cmake_minimum_required(VERSION 3.15)

# store the version of the core library
set(GLGE_CORE_VERSION 0.1.0)

# actual project creation
project(GLGE_CORE_PROJ LANGUAGES CXX)

# for releases use optimizations
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall")
# for debug use all warnings
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fms-extensions -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow -Wdouble-promotion -Wformat=2 -Wnull-dereference -Wundef -Wcast-align -Wmissing-declarations -Wunreachable-code -Winit-self -Wlogical-op")

# if no build type is set, select release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# store all files for the core
set(GLGE_CORE_SRC 
    Messages/MessageListener.cpp
    
    Events/EventHandler.cpp
    Events/Event.cpp

    Layers/LayerStack.cpp

    Application/App.cpp

    Setting/Settings.cpp
)

# create the library
add_library(GLGE_CORE ${GLGE_CORE_SRC})

# Apply to all targets in the project
if(MSVC)
    # Use dynamic runtime: /MD (Release), /MDd (Debug)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

# use C++ 20
set_target_properties(GLGE_CORE PROPERTIES 
                        CMAKE_CXX_STANDARD 20 
                        CMAKE_CXX_STANDARD_REQUIRED ON 
                        CMAKE_CXX_EXTENSIONS ON 
                        VERSION ${GLGE_CORE_VERSION} 
                        SOVERSION 1)

# add the sub-libraries
## GLGE_BG
add_subdirectory(${PROJECT_SOURCE_DIR}/../GLGE_BG ${PROJECT_BINARY_DIR}/../GLGE_BG)
target_link_libraries(GLGE_CORE PUBLIC GLGE_BG)
## PugiXML
# disable debugging for PugiXML
set(CMAKE_CXX_FLAGS_DEBUG "-O2")
# include actual PugiXML
add_subdirectory(${PROJECT_SOURCE_DIR}/../external/pugixml ${PROJECT_BINARY_DIR}/../external/pugixml)
target_link_libraries(GLGE_CORE PRIVATE pugixml)
target_include_directories(GLGE_CORE PRIVATE ${PROJECT_SOURCE_DIR}/../external/pugixml/src)