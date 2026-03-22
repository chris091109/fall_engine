cmake_minimum_required(VERSION 3.20)
project(fall_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(PkgConfig REQUIRED)

find_path(GLM_INCLUDE_DIR
    NAMES glm/glm.hpp
    PATHS
        ${CMAKE_SOURCE_DIR}/include/glm
        /usr/include
        /usr/local/include
)

include(FetchContent)

FetchContent_Declare(
    cgltf
    GIT_REPOSITORY https://github.com/jkuhlmann/cgltf.git
    GIT_TAG        v1.14
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(cgltf)

find_program(GLSLC glslc REQUIRED)

add_subdirectory(engine)
add_subdirectory(game)
