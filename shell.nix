{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell
{
    # Remove compiler flags for -O warnings
    hardeningDisable =
    [
        "fortify"
    ];

    packages =
    [
        pkgs.gcc
        pkgs.cmake
        pkgs.libffi
    ];

    # Include the packages that glfw uses
    inputsFrom =
    [
        pkgs.glfw
    ];
}
