# CMake definition
cmake_minimum_required(VERSION 3.15)

# Project definition
project(MinecraftClone DESCRIPTION "A simple Minecraft clone" LANGUAGES C CXX)

# Use C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Top-level variables
set(LIB_DIR "lib")

# === Application ===
add_subdirectory("src")

# === OpenGL ===
find_package(OpenGL REQUIRED)

# === GLFW ===
set(GLFW_DIR "${LIB_DIR}/glfw")

# Turn off redundant build options
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

add_subdirectory("${GLFW_DIR}")

# === GLAD ===
set(GLAD_DIR "${LIB_DIR}/glad")

# Create a library from GLAD files since it doesn't come with CMake
add_library(glad "${GLAD_DIR}/src/glad.c")
target_include_directories(glad PUBLIC "${GLAD_DIR}/include")

# === GLM ===
set(GLM_DIR "${LIB_DIR}/glm")
add_subdirectory("${GLM_DIR}")

# === stb_image ===
set(STB_IMAGE_DIR "${LIB_DIR}/stb_image")

# Create a library from stb_image files since it doesn't come with CMake
add_library(stb-image "${STB_IMAGE_DIR}/stb_image.cpp")
target_include_directories(stb-image PUBLIC "${STB_IMAGE_DIR}")

# === Perlin Noise ===
set(PERLIN_NOISE_DIR "${LIB_DIR}/PerlinNoise")

# Create a library from PerlinNoise files since it doesn't come with CMake
add_library(perlin-noise "${PERLIN_NOISE_DIR}/PerlinNoise.cpp")
target_include_directories(perlin-noise PUBLIC "${PERLIN_NOISE_DIR}")
