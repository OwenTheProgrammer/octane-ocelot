#include "filesys/file_io.h"
#include "filesys/endian.h"
#include "filesys/oct.h"
#include "ocelot/math_utils.h"
#include "ocelot/memory.h"

static void _test_math_utils()
{
    quaternion q = (quaternion){0.0, -0.9969769, 0.0, -0.07769856};
    vec3f t = (vec3f){37.958584, 47.926933, 346.99725};

    mat4x4 TRS = mat4x4_combine_Q_T(q, t);
    print_mat4x4(TRS);

    vec3f euler_angles = quaternion_to_euler_radians(q);
    vec3f euler_degrees = rad2deg_vec3f(euler_angles);
    print_vec3f_prefixed(euler_degrees, "deg");

    mat3x3 rotor = eulers_to_rotor3x3(euler_angles);
    print_mat3x3(rotor);
    vec3f computed_radians = mat3x3_get_rotation(rotor);
    print_vec3f_prefixed(rad2deg_vec3f(computed_radians), "deg");
}

int main()
{
    //Read and convert the oct file to little endian
    oct_file oct = oct_read_file("oilrig.oct", ENDIAN_LITTLE);

    //oct_print_header(oct.header);

    //We want to export as big endian
    endian_set_target(ENDIAN_LITTLE);
    ocl_dbuf header = oct_write_buffer(&oct);

    ocl_dbuf_export(&header, "bin/oilrig_exported.oct");
    ocl_dbuf_free(&header);


    oct_free_file(&oct);
    return 0;
}
