#pragma once
#include <stdint.h>

#define FS_FILEPATH_MAX 1024u

typedef struct filepath_t   filepath_t;

struct filepath_t
{
    uint32_t length;

    char path[FS_FILEPATH_MAX];
};

/* Generate a filepath from a string */
filepath_t fs_path_create(const char* path);

/* Creates an absolute filepath from a relative filepath string */
filepath_t fs_path_create_absolute(const char* relative);

/* Gets the directory of a filepath */
filepath_t fs_path_get_directory(filepath_t path);

/* Helper function to get the absolute work directory from the `argv` value */
filepath_t fs_path_cwd_from_argv(const char* argv);


filepath_t fs_path_top_filename(filepath_t path);

filepath_t fs_path_combine(filepath_t lhs, filepath_t rhs);

filepath_t _fs_path_concat(filepath_t path, const char* format, ...);

#define fs_path_concat(path, format, ...) _fs_path_concat(path, format, ##__VA_ARGS__)

