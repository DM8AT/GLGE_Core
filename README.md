# About
This library is the core library of the GLGE library set. It is used to bind all user-facing APIs together. The main focus of the library is the construction of a comunication interface that can be used to comunice between the libraries, the user and the core application. 
This interface is designed to be easy to use and fast. 

# Dependencies
## GLGE Dependencies
- [GLGE Math](https://github.com/DM8AT/GLGE_Math)
- [GLGE BG](https://github.com/DM8AT/GLGE_BG)
## External dependencies
 - [mustache ECS](https://github.com/kirillochnev/mustache)
  - [Easy Profiler](https://github.com/yse/easy_profiler)
 - [PugiXML](https://github.com/zeux/pugixml)
 - [Assimp](https://github.com/assimp/assimp)
 - [ZLib](https://github.com/madler/zlib)

# Function list

| Class name | C++ interface | C interface | First introduced in | Last updated in |
|------------|---------------|-------------|---------------------|-----------------|
| App        | :white_check_mark:|:white_check_mark:| 0.1.0      | 0.1.0           |
| BaseAsset  | :white_check_mark:|:x:      | 0.1.0               | 0.1.0           |
| AssetManager| :white_check_mark:| :x:    | 0.1.0               | 0.1.0           |
| Event      | :white_check_mark:| :warning: | 0.1.0             | 0.1.0           |
| EventHandler | :white_check_mark:| :white_check_mark: | 0.1.0  | 0.1.0           |
| Compression | :white_check_mark:|:white_check_mark:| 0.1.0     | 0.1.0           |
| Encryption | :white_check_mark: |:white_check_mark:| 0.1.0     | 0.1.0           |
| File       | :white_check_mark: | :x:   | 0.1.0                | 0.1.0           |
| FileDecorator | :white_check_mark: | :white_check_mark: | 0.1.0| 0.1.0           |
| Object     | :white_check_mark: | :x:   | 0.1.0                | 0.1.0           |
| Scene      | :white_check_mark: | :x:   | 0.1.0                | 0.1.0           |
| System     | :white_check_mark: | :x:   | 0.1.0                | 0.1.0           |
| Transform  | :white_check_mark: | :white_check_mark: | 0.1.0   | 0.1.0           |
| Mesh       | :white_check_mark: | :white_check_mark: | 0.1.0   | 0.1.0           |
| MeshAsset  | :white_check_mark: | :x:   | 0.1.0                | 0.1.0           |
| Triangle   | :white_check_mark: | :warning: | 0.1.0            | 0.1.0           |
| VertexElement | :white_check_mark: | :warning: | 0.1.0         | 0.1.0           |
| VertexLayout | :white_check_mark: | :white_check_mark: | 0.1.0 | 0.1.0           |
| AABB       | :white_check_mark: | :white_check_mark: | 0.1.0   | 0.1.0           |
| BVH        | :white_check_mark: | :x:   | 0.1.0                | 0.1.0           |
| Sphere     | :white_check_mark: | :x:   | 0.1.0                | 0.1.0           |
| Layer / LayerBase | :white_check_mark: | :white_check_mark: | 0.1.0 | 0.1.0      |
| LayerStack | :white_check_mark: | :white_check_mark: | 0.1.0   | 0.1.0           |
| Message    | :white_check_mark: | :white_check_mark: | 0.1.0   | 0.1.0           |
| MessageListener | :white_check_mark: | :white_check_mark: | 0.1.0 | 0.1.0        |
| Settings   | :white_check_mark: | :white_check_mark: | 0.1.0   | 0.1.0           |

# Compiling
[![CMake on multiple platforms](https://github.com/DM8AT/GLGE_Core/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/DM8AT/GLGE_Core/actions/workflows/cmake-multi-platform.yml)
## Building the library
To compile the library, simply clone the library from the directory. Make sure to clone recursivly to gather all submodules (simply run `git clone --recurse-submodules  https://github.com/DM8AT/GLGE_Core.git`). Make sure that in the folder you clone, you also clone the 
[Math](https://github.com/DM8AT/GLGE_Math) and the [BG](https://github.com/DM8AT/GLGE_BG) library as this library expects to find them there. If they are not located there, the building will fail. This design choise is so not every GLGE library that depends on this one has 
to include it itself, but they can rely on a shared library. 
To build the library static, create a build directory, change into it and simply run `cmake ..` and then `cmake --build .`. This will build the static library into the build directory. 
## Integration into your CMake project
To integrate the library into your own CMake project, follow the setup steps from [the building steps](#building_the_library) untill the building. Then, simply add the directory you cloned the GLGE Core library to as a submodule to your CMake project: 
`add_subdirectory("path/to/GLGE_Core")` and then link the `GLGE_CORE` library to your project. This will link the dependencies automatically. 

# Current OS support
The library itself does not contain any operating system dependend code. But the [GLGE BG](https://github.com/DM8AT/GLGE_BG) library contains some OS dependend support. Check [this section](https://github.com/DM8AT/GLGE_BG?tab=readme-ov-file#current-os-support) to find out 
more. 
