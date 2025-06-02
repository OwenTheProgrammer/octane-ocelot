#include "ocelot/engine/window.h"

int main()
{
    if(!ocl_gui_init())
        return -1;

    ocl_gui_loop();

    return 0;
}
