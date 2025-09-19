# set the minimum required version
cmake_minimum_required(VERSION 3.10)

# store the version of the core library
set(GLGE_CORE_VERSION 0.1.0)

# actual project creation
project(GLGE_CORE_PROJ LANGUAGES CXX)

# for releases use optimizations
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# if no build type is set, select release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# store all files for the core
set(GLGE_CORE_SRC 
    Messages/MessageListener.cpp
)

# create the library
add_library(GLGE_CORE ${GLGE_CORE_SRC})

# use C++ 20
set_target_properties(GLGE_CORE PROPERTIES 
                        CXX_STANDARD 20 
                        CXX_STANDARD_REQUIRED ON 
                        VERSION ${GLGE_CORE_VERSION} 
                        SOVERSION 1)