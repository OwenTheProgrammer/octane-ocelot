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
        pkgs.gnumake
        pkgs.cmake
    ];

    # Include the packages that glfw uses
    inputsFrom =
    [
        pkgs.glfw
    ];
}
