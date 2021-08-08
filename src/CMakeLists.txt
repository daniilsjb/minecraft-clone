# Add sources to the game's executable
add_executable(game
    "common/db_perlin.cpp"
    "common/direction.hpp"
    "common/ray.cpp"
    "common/ray.hpp"
    "common/stb_image.cpp"
    "common/types.hpp"

    "gfx/camera.cpp"
    "gfx/camera.hpp"
    "gfx/renderer.cpp"
    "gfx/renderer.hpp"
    "gfx/shader.cpp"
    "gfx/shader.hpp"
    "gfx/texture.cpp"
    "gfx/texture.hpp"
    "gfx/vertex_array.cpp"
    "gfx/vertex_array.hpp"
    "gfx/vertex_buffer.cpp"
    "gfx/vertex_buffer.hpp"
    "gfx/vertex_layout.cpp"
    "gfx/vertex_layout.hpp"
    "gfx/window.cpp"
    "gfx/window.hpp"

    "world/block.hpp"
    "world/block_mesh.cpp"
    "world/block_mesh.hpp"
    "world/chunk.cpp"
    "world/chunk.hpp"
    "world/chunk_mesh.cpp"
    "world/chunk_mesh.hpp"
    "world/generator.cpp"
    "world/generator.hpp"
    "world/player.cpp"
    "world/player.hpp"
    "world/world.cpp"
    "world/world.hpp"

    "main.cpp"
    "state.hpp"
)

# Specify language standard
set_target_properties(game PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Link the executable with additional properties
target_link_libraries(game PRIVATE
    compiler-warnings
)

# Link the executable with the libraries
target_link_libraries(game PRIVATE
    glfw ${GLFW_LIBRARIES}
    glad ${CMAKE_DL_LIBS}
    glm
    stb-image
    db-perlin
)

# We're using a custom OpenGL extension loader library,
# so turn off GLFW's default OpenGL header inclusion
target_compile_definitions(game PRIVATE GLFW_INCLUDE_NONE)

# Copy resources to the build directory
add_custom_command(TARGET game POST_BUILD
    COMMENT "Copying resources to the build directory"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/res" "$<TARGET_FILE_DIR:game>/res"
)
