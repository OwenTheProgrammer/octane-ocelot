
add_subdirectory(glad)

set(CGLM_SHARED OFF CACHE BOOL "Build static libraries" FORCE)
set(CGLM_STATIC ON CACHE BOOL "Static build" FORCE)
add_subdirectory(cglm)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target" FORCE)
add_subdirectory(glfw)