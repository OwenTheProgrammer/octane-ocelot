cmake_minimum_required(VERSION 3.21)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)

    # Default to Debug
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

    # Set the possible target build types for cmake GUI
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")

endif()

# CGLM cmake options
set(CGLM_SHARED OFF CACHE BOOL "Build static libraries" FORCE)
set(CGLM_STATIC ON CACHE BOOL "Static build" FORCE)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# GLFW cmake options
#set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation" FORCE)
#set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target" FORCE)

# Ocelot cmake options
option(OCELOT_BUILD_CLI "Build the command line interface tool" ON)
option(OCELOT_BUILD_EDITOR "Build the GUI based editor tool" ON)

# Generate clangd index file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # build debug testing
    set(OCELOT_BUILD_TESTING True)

    add_compile_options(
        -Wall
        -std=gnu17
        -O0
        -ggdb
        -Wno-unused-function
        -Wno-unused-variable
    )

    #add_compile_options(-fsanitize=address)
    #add_link_options(-fsanitize=address)
else()
    set(OCELOT_BUILD_TESTING False)

    add_compile_options(
        -std=gnu17
        -O3
        -Wno-unused-result
    )
endif()

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

if(OCELOT_BUILD_CLI)
    message(STATUS "Including ocelot CLI")
endif()
if(OCELOT_BUILD_EDITOR)
    message(STATUS "Including ocelot editor")
endif()
if(OCELOT_BUILD_TESTING)
    message(STATUS "Including ocelot debug testing")
endif()

add_subdirectory(libs/dbuf)
add_subdirectory(libs/file-path)

add_subdirectory(src/ocelot)
add_subdirectory(src/octane)
add_subdirectory(src/platform)

#add_custom_target(asset_copy ALL)
#add_custom_command(
#    TARGET asset_copy POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
#    COMMENT "Copying assets directory"
#)

if(OCELOT_BUILD_CLI)

    project(ocelot_cli LANGUAGES C)

    add_executable(${PROJECT_NAME} src/main_cli.c)

    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            ocelot octane
    )

endif()

if(OCELOT_BUILD_EDITOR)

    project(ocelot_editor LANGUAGES C)

    add_subdirectory(deps/glad)
    add_subdirectory(deps/cglm)
    add_subdirectory(deps/glfw)
#add_subdirectory(src/engine/_old)
    add_subdirectory(src/engine)

    add_executable(${PROJECT_NAME} src/main_editor.c)

    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            engine
    )

    #add_custom_command(
    #    TARGET ${PROJECT_NAME} POST_BUILD
    #    COMMAND ${CMAKE_COMMAND} -E copy_directory
    #    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    #)
    file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

endif()

if(OCELOT_BUILD_TESTING)

    project(ocelot_testing LANGUAGES C)

    add_executable(${PROJECT_NAME} src/main_test.c)

    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            octane ocelot
    )

endif()


# Copy the testing game files into the bin dir
if(EXISTS ${CMAKE_SOURCE_DIR}/gamefiles)
    file(COPY ${CMAKE_SOURCE_DIR}/gamefiles DESTINATION ${CMAKE_BINARY_DIR})
endif()

