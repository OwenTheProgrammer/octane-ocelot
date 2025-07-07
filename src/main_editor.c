#include "engine/core/runtime.h"
#include "engine/core/window.h"
#include <stdlib.h>

int main(int argc, const char* argv[])
{
    oce_init(argv[0]);

    if(!_rt.initialized)
        return EXIT_FAILURE;

    //oce_load("gamefiles/pc/worlds/testing");
    //oce_load("gamefiles/xbox360/worlds/oilrig");

    oce_gui_loop();

    oce_free();
    return EXIT_SUCCESS;
}
