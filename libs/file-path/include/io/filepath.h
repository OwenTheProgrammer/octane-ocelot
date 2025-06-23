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

filepath_t fs_path_top_dirname(filepath_t path);

filepath_t _fs_path_concat(filepath_t path, const char* format, ...);

#define fs_path_concat(path, format, ...) _fs_path_concat(path, format, ##__VA_ARGS__)

