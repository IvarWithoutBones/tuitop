{ pkgs ? import <nixpkgs> {} }:

# TODO: for some reason this is still using gcc??
pkgs.clangStdenv.mkDerivation rec {
  pname = "tuitop";
  version = "0";

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
