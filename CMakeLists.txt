cmake_minimum_required(VERSION 3.10)

project(MAIN_PROJ LANGUAGES CXX)

add_executable(MAIN src/main.cpp)

add_subdirectory(src/GLGECore)
target_link_libraries(MAIN GLGE_CORE)


set_target_properties(MAIN PROPERTIES 
                        CXX_STANDARD 20 
                        CXX_STANDARD_REQUIRED ON)