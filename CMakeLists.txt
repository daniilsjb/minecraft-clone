# CMake definition
cmake_minimum_required(VERSION 3.15)

# Project definition
project(minecraft-clone DESCRIPTION "A Minecraft-like game made for fun." LANGUAGES C CXX)

# Compiler warnings
include(cmake/CompilerWarnings.cmake)
add_library(compiler-warnings INTERFACE)
set_project_warnings(compiler-warnings)

# Libraries
add_subdirectory(lib)

# Main program
add_subdirectory(src)
