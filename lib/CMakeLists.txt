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

# A little hack, but because glm is a header-only library it gets compiled together with the code
# that includes it, which means it may emit warnings. This should make some compilers treat its
# include directory as belonging to the system, suppressing unnecessary diagnostics.
set_target_properties(glm PROPERTIES
    INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:glm,INTERFACE_INCLUDE_DIRECTORIES>
)

# stb_image
add_subdirectory("stb_image")

# db_perlin
add_subdirectory("db_perlin")