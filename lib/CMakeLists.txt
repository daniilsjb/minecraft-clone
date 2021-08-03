# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

add_subdirectory("glfw")

# GLAD
add_subdirectory("glad")

# GLM
add_subdirectory("glm")

# stb_image
add_subdirectory("stb_image")

# db_perlin
add_subdirectory("db_perlin")