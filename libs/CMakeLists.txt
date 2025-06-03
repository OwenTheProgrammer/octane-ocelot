
add_subdirectory(glad)

set(CGLM_SHARED OFF CACHE BOOL "Build static libraries" FORCE)
set(CGLM_STATIC ON CACHE BOOL "Static build" FORCE)
add_subdirectory(cglm)
