cmake_minimum_required(VERSION 3.21)
project(ocelot LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    
    # Default to Debug
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

    # Set the possible target build types for cmake GUI
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")

endif()

message("BuildType: ${CMAKE_BUILD_TYPE}")

# Build with:
# cmake -DCMAKE_BUILD_TYPE=Debug -B bin/ -S .
# cmake --build bin/ -j

# set(CMAKE_C_COMPILER gcc)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-Wall -std=gnu17 -O0 -ggdb -Wno-unused-function -Wno-unused-variable)

    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

add_subdirectory(libs)

add_subdirectory(src)

add_executable(ocelot_cli src/main_cli.c)

target_link_libraries(ocelot_cli
    ocelot octane
)

add_executable(ocelot_editor src/main_editor.c)

target_link_libraries(ocelot_editor
    glad ocelot octane engine cglm
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(testing src/main_test.c)

    target_link_libraries(testing
        glad ocelot octane engine cglm
    )
endif()


file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

if(EXISTS ${CMAKE_SOURCE_DIR}/xbfiles)
    file(COPY ${CMAKE_SOURCE_DIR}/xbfiles DESTINATION ${CMAKE_BINARY_DIR})
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET ocelot_editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}
    )
endif()
