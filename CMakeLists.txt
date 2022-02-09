cmake_minimum_required(VERSION 3.20)
project(tui-top LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For the LSP
set(DCMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tui-top
    src/main.cpp
    src/proc_info.cpp
    src/ui.cpp
)

target_compile_options(tui-top PRIVATE
    -Wall
    -Wno-reorder
    -Wno-missing-braces
    -Wno-unused-variable
    -Wno-dangling-else
)

find_package(PkgConfig)
pkg_check_modules(libprocps REQUIRED IMPORTED_TARGET libprocps)

find_package(fmt REQUIRED)
find_package(ftxui REQUIRED)

target_link_libraries(tui-top
    PRIVATE PkgConfig::libprocps
    PRIVATE fmt::fmt
    PRIVATE ftxui::screen
    PRIVATE ftxui::component
    PRIVATE ftxui::dom
)
