# source/game/CMakeLists.txt

file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(fall_game ${GAME_SOURCES})

target_include_directories(fall_game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR} 
)

# helix_engine is PUBLIC so all its includes + libs propagate here
target_link_libraries(fall_game PRIVATE helix_engine)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(fall_game PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wno-missing-field-initializers
    )
endif()

# Install
install(TARGETS fall_game RUNTIME DESTINATION bin)

install(DIRECTORY ${SPIRV_OUTPUT_DIR}/
    DESTINATION share/fall_engine/shaders
    FILES_MATCHING PATTERN "*.spv"
)
