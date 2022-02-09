{ pkgs ? import <nixpkgs> {} }:

let
  _ftxui = (pkgs.ftxui.overrideAttrs (attrs: {
    src = builtins.fetchTarball "https://github.com/ArthurSonzogni/FTXUI/archive/refs/tags/v2.0.0.tar.gz";
  }));
in
pkgs.clangStdenv.mkDerivation rec {
  name = "tui-top";

  nativeBuildInputs = with pkgs; [
    cmake
    pkg-config
  ];

  buildInputs = with pkgs; [
    _ftxui
    fmt
    procps
  ];
}
