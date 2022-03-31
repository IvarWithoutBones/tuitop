cmake_minimum_required(VERSION 3.20)
project(tuitop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig)
find_package(fmt REQUIRED)
find_package(ftxui REQUIRED)
pkg_check_modules(libprocps REQUIRED IMPORTED_TARGET libprocps)

# For the LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tuitop
    src/main.cpp
    src/proc_info.cpp
    src/ui.cpp
)

target_compile_options(tuitop PRIVATE
    -Wall
    -Wno-reorder
    -Wno-missing-braces
    -Wno-unused-variable
    -Wno-dangling-else
)

target_link_libraries(tuitop
    PRIVATE PkgConfig::libprocps
    PRIVATE fmt::fmt
    PRIVATE ftxui::screen
    PRIVATE ftxui::component
    PRIVATE ftxui::dom
)

install(TARGETS tuitop DESTINATION bin)
