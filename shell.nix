{ pkgs ? import <nixpkgs> {} }:

pkgs.clangStdenv.mkDerivation rec {
  name = "tui-top";

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
