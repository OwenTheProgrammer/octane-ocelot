#include "io/filepath.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifdef PLATFORM_WINDOWS
    static const char _TARGET_DS = '\\';
    static const char _OTHER_DS = '/';
#else
    static const char _TARGET_DS = '/';
    static const char _OTHER_DS = '\\';
#endif //PLATFORM_WINDOWS

static void _str_replace_chr(char* string, char find, char replace)
{
    while( (string = strchr(string, find)) )
    {
        *(string++) = replace;
    }
}

filepath_t fs_path_create(const char* path)
{
    filepath_t fp = (filepath_t){0};

    strncpy(fp.path, path, FS_FILEPATH_MAX);
    _str_replace_chr(fp.path, _OTHER_DS, _TARGET_DS);

    fp.length = strnlen(fp.path, FS_FILEPATH_MAX);

    return fp;
}

static filepath_t _path_concat(filepath_t lhs, const char* rhs)
{
    //Add the rhs buffer onto the end
    strncat(lhs.path, rhs, FS_FILEPATH_MAX - lhs.length);

    //Convert the seperators if needed
    _str_replace_chr(lhs.path + lhs.length, _OTHER_DS, _TARGET_DS);

    //Update the string length
    lhs.length = strnlen(lhs.path, FS_FILEPATH_MAX);

    return lhs;
}

filepath_t _fs_path_concat(filepath_t path, const char* format, ...)
{
    char ext[FS_FILEPATH_MAX];

    va_list args;
    va_start(args, format);

    vsnprintf(ext, FS_FILEPATH_MAX, format, args);

    va_end(args);

    //Add up the amount of slashes
    int p_end = path.path[path.length-1] == _TARGET_DS;
    int s_end = ext[0] == _TARGET_DS || ext[0] == _OTHER_DS;
    int sep_count = p_end + s_end;

    if(sep_count == 0)
    {
        path.path[path.length++] = _TARGET_DS;
    }
    else if(sep_count > 1)
    {
        path.path[path.length-1] = '\0';
        path.length--;
    }

    return _path_concat(path, ext);
}

filepath_t fs_path_top_dirname(filepath_t path)
{
    uint32_t end_len = path.length;
    end_len -= path.path[path.length-1] == _TARGET_DS;

    char* end = path.path + end_len;

    char buffer[FS_FILEPATH_MAX];
    strncpy(buffer, path.path, FS_FILEPATH_MAX);

    char* start = strrchr(path.path, _TARGET_DS);

    filepath_t result = (filepath_t){0};

    if(start == NULL)
    {
        result.length = end_len;
        start = buffer;
    }
    else
    {
        result.length = end - start - 1;
        start++;
    }

    memcpy(result.path, start, result.length);

    return result;
}
