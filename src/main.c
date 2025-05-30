#include "byteconv/endian.h"
#include "octane/oct.h"
#include <stdio.h>
#include <string.h>

static void parse_file_path(char(* dst)[FILENAME_MAX], const char* arg)
{
    if(arg[0] == '\"' || arg[0] == '\'')
    {
        strncpy(*dst, arg + 1, strnlen(arg + 1, FILENAME_MAX)-1);
    }
    else
    {
        strncpy(*dst, arg, strnlen(arg, FILENAME_MAX));
    }
}

int main(int argc, const char* argv[])
{
    if(argc != 4)
    {
        printf("Usage: ./ocelot <path to input oct> <path to output oct> <endian: l|b>\n");
        return 0;
    }

    char input_file[FILENAME_MAX] = { [0 ... FILENAME_MAX-1] = 0 };
    char output_file[FILENAME_MAX] = { [0 ... FILENAME_MAX-1] = 0 };
    endian_t target_endian = argv[3][0] == 'l' ? ENDIAN_LITTLE : ENDIAN_BIG;

    parse_file_path(&input_file, argv[1]);
    parse_file_path(&output_file, argv[2]);

    printf("input oct: %s\n", input_file);
    printf("output_oct: %s\n", output_file);
    printf("target endian: %s\n\n", _ENDIAN_PRINT_TABLE[(int)target_endian]);

    // Load the oct file
    //oct_file oct = oct_load_file("bin/oilrig.oct");
    oct_file oct = oct_load_file(input_file);

    // Set the target endian to big
    //endian_set_target(ENDIAN_LITTLE);
    endian_set_target(target_endian);

    //oct_store_file(oct, "bin/output.oct");
    oct_store_file(oct, output_file);

    oct_file_free(&oct);

    return 0;
}
