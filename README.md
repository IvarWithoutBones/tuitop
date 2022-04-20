<div align="center">

# tuitop

<img src="https://github.com/ivarwithoutbones/tuitop/workflows/Build/badge.svg">

A terminal based task manager written in C++, with support for vim like keybindings. Licensed under the [GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html/).

</div>

Support for x86_64-linux and aarch64-linux support is actively maintained, and there are continuous integration builds done by [github actions](https://github.com/IvarWithoutBones/tuitop/actions). This project is mainly made to teach myself C++, its my first (working) application. It is a work-in-progress, so expect limited functionality and many bugs :)

## Compiling
Here are the dependencies:
* [CMake](https://cmake.org/)
* [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/)
* [procps](https://gitlab.com/procps-ng/procps/)
* [fmt](http://fmtlib.net/)
* [FTXUI](https://github.com/ArthurSonzogni/FTXUI/)

The recommanded way to run tuitop is using [nix](https://nixos.org/), which can deterministically resolves all the dependencies for you. A [nix flake](https://nixos.org/manual/nix/unstable/command-ref/new-cli/nix3-flake.html) is provided, for more information see [`./flake.nix`](https://github.com/IvarWithoutBones/tuitop/blob/main/flake.nix). You can use it as follows: 
 
 * Compile the latest commit pushed to the `main` branch:
   ```shell
   $ nix build github:IvarWithoutBones/tuitop
   ```
 * Compile and run a specific commit: 
   ```shell
   $ nix run github:IvarWithoutBones/tuitop/19ac9b7436d085491dc0e49cfa2454529831c27f
   ```
   
On other distrobutions you can compile it as follows:
```shell
~/tuitop $ mkdir -p build && cd build
~/tuitop $ cmake ..
~/tuitop $ make -j
~/tuitop $ ./tuitop
```
