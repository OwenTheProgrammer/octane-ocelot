#include "engine/core/runtime.h"
#include "engine/core/scene.h"
#include "engine/core/window.h"
#include "io/filepath.h"
#include "ocelot/scene/data.h"
#include "octane/oct/oct.h"
#include "octane/oct/scene_descriptor.h"
#include "engine/internal.h"
#include "console/log.h"

oce_engineRuntime _rt = (oce_engineRuntime){0};

static void _init_filepaths(const char* argv)
{
    _rt.path.work_dir = fs_path_cwd_from_argv(argv);

    LOG_INFO("Work directory: " OCL_FILEPATH_FMT "\n", _rt.path.work_dir.path);
    //printf("[engine]: Work directory: \"%s\"\n", _rt.path.work_dir.path);

    _rt.path.assets_dir = fs_path_combine(_rt.path.work_dir, fs_path_create("assets"));
    //printf("[engine]: Assets directory: \"%s\"\n", _rt.path.assets_dir.path);
    LOG_INFO("Assets directory: " OCL_FILEPATH_FMT "\n", _rt.path.assets_dir.path);
}


void oce_init(const char* argv)
{
    _rt.initialized = 0;

    _init_filepaths(argv);

    if(!_oce_init_glfw())
        return;

    _rt.initialized = 1;
}

void oce_load(const char* assets_dir)
{
    // Get the asset directory path
    filepath_t asset_path = fs_path_create(assets_dir);
    _rt.path.game_dir = fs_path_combine(_rt.path.work_dir, asset_path);

    LOG_INFO("Loading assets from " OCL_FILEPATH_FMT "\n", _rt.path.game_dir.path);
    //printf("[Engine]: Loading assets from \"%s\"\n", _rt.path.game_dir.path);

    //Load the oct file
    oct_file oct = oct_load_file(_rt.path.game_dir.path);

    //Serialize the scene structure
    oct_sceneDescriptor oct_descriptor = oct_parse_scene_tree(oct);
    ocl_sceneData scene_data = ocl_load_scene_data(&oct_descriptor, &oct);

    //Load the scene
    oce_scene scene = oce_load_scene(&scene_data);
    scene.descriptor = oct_descriptor;


    //ocl_free_scene_data(&scene_data);
    //oct_free_scene_descriptor(&oct_descriptor);
    oct_free_file(&oct);

    _rt.loaded_scene = scene;
}

void oce_free()
{
    oct_free_scene_descriptor(&_rt.loaded_scene.descriptor);
    ocl_free_scene_data(&_rt.loaded_scene.oct_data);
    oce_free_scene(&_rt.loaded_scene);
    oce_gui_free();
}
