# source/engine/CMakeLists.txt

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "*.cpp")

add_library(helix_engine STATIC ${ENGINE_SOURCES})

target_compile_definitions(helix_engine PUBLIC
    SHADER_PATH="${CMAKE_BINARY_DIR}/shaders/"
)

target_precompile_headers(helix_engine PUBLIC
    <vulkan/vulkan.h>
    <GLFW/glfw3.h>
    <glm/glm.hpp>
    <vector>
    <string>
    <memory>
    <array>
    <span>
    <iostream>
    <print>
    "common.h"
)

target_include_directories(helix_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}  
    ${CMAKE_SOURCE_DIR}/include
    ${Vulkan_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
    ${cgltf_SOURCE_DIR}
)

target_link_libraries(helix_engine PUBLIC
    glfw
    Vulkan::Vulkan
    ${CMAKE_DL_LIBS}
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(helix_engine PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wno-missing-field-initializers
    )
endif()

target_compile_definitions(helix_engine PUBLIC
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

# ---------------------------------------------------------------------------
# Shaders — engine owns the shader compilation, exposes SPIRV path via var
# ---------------------------------------------------------------------------
set(SPIRV_OUTPUT_DIR "${CMAKE_BINARY_DIR}/shaders" CACHE PATH "SPIR-V output directory")
file(MAKE_DIRECTORY ${SPIRV_OUTPUT_DIR})

file(GLOB_RECURSE GLSL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/drivers/shaders/*.glsl"
)

foreach(GLSL ${GLSL_SOURCES})
    get_filename_component(FILE_NAME ${GLSL} NAME_WE)
    set(SPIRV "${SPIRV_OUTPUT_DIR}/${FILE_NAME}.spv")

    if(FILE_NAME STREQUAL "vert")
        set(STAGE vertex)
    elseif(FILE_NAME STREQUAL "frag")
        set(STAGE fragment)
    elseif(FILE_NAME STREQUAL "comp")
        set(STAGE compute)
    else()
        message(WARNING "Unknown shader stage for: ${FILE_NAME}.glsl — skipping")
        continue()
    endif()

    add_custom_command(
        OUTPUT  ${SPIRV}
        COMMAND ${GLSLC} -fshader-stage=${STAGE} ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL}
        COMMENT "Compiling shader: ${FILE_NAME}.glsl"
        USES_TERMINAL
    )
    list(APPEND SPIRV_OUTPUTS ${SPIRV})
endforeach()

add_custom_target(helix_shaders ALL DEPENDS ${SPIRV_OUTPUTS})
add_dependencies(helix_engine helix_shaders)
