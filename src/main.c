#include "gui/window.h"

int main(int argc, const char* argv[])
{
    if(!ocl_gui_init())
        return -1;

    ocl_gui_loop();

    return 0;
}
