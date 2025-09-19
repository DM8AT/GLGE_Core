cmake_minimum_required(VERSION 3.10)

project(MAIN LANGUAGES CXX)

add_executable(MAIN src/main.cpp)

add_subdirectory(src/GLGECore)
target_link_libraries(MAIN GLGE_CORE)