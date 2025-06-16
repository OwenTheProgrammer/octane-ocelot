{ pkgs ? import <nixpkgs> {} }:

let
    mingw = pkgs.pkgsCross.mingwW64;
    xorg = pkgs.xorg;
in
    pkgs.mkShell
    {
        hardeningDisable = [ "all" ];
        buildInputs =
        [
            pkgs.gcc
            pkgs.gnumake
            pkgs.cmake

            # Packages for GLFW (since pkgs.glfw doesnt support static linking)
            pkgs.libxkbcommon
            pkgs.libGL
            xorg.libXrandr
            xorg.libXcursor
            xorg.libXinerama
            xorg.libX11
            xorg.libXi

            mingw.stdenv.cc
        ];
    }
