{ pkgs ? import <nixpkgs> {} }:

let
    mingw = pkgs.pkgsCross.mingwW64;
in
    pkgs.mkShell
    {
        buildInputs =
        [
            pkgs.gcc
            pkgs.gnumake

            mingw.stdenv.cc
        ];
    }
