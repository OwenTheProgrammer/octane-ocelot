#include "io/filepath.h"
#include <stdlib.h>
#include <string.h>

filepath_t fs_path_create_absolute(const char* relative)
{
    filepath_t path = fs_path_create(relative);

    filepath_t absolute = (filepath_t){0};
    realpath(path.path, absolute.path);
    absolute.length = strnlen(path.path, FS_FILEPATH_MAX);

    return absolute;
}
