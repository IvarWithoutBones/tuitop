{ pkgs ? import <nixpkgs> {} }:

pkgs.clangStdenv.mkDerivation rec {
  name = "tuitop";

  nativeBuildInputs = with pkgs; [
    cmake
    pkg-config
  ];

  buildInputs = with pkgs; [
    ftxui
    fmt
    procps
  ];
}
