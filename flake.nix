{
  description = "A tui top clone written in C++";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs, flake-utils }:
    let
      version = builtins.substring 0 8 self.lastModifiedDate;

      # All supported systems
      forAllSystems = nixpkgs.lib.genAttrs [ "x86_64-linux" "aarch64-linux" ];

      # Nixpkgs instantiated for each supported system
      nixpkgsFor = forAllSystems (system: import nixpkgs { inherit system; overlays = [ self.overlay ]; });
    in {
      overlay = final: prev: {
        tuitop = with final; clangStdenv.mkDerivation rec {
          pname = "tuitop";
          inherit version;

          src = ./.;

          nativeBuildInputs = [
            cmake
            pkg-config
          ];

          buildInputs = [
            ftxui
            fmt
            procps
          ];
        };
      };

      packages = forAllSystems (system: {
        inherit (nixpkgsFor.${system}) tuitop;
      });

      defaultPackage = forAllSystems (system: self.packages.${system}.tuitop);
    };
}
