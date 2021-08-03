# Add sources to the game's executable
add_executable(game
    "main.cpp"
    "State.hpp"

    "gfx/Camera.cpp"
    "gfx/Camera.hpp"
    "gfx/Renderer.cpp"
    "gfx/Renderer.hpp"
    "gfx/Shader.cpp"
    "gfx/Shader.hpp"
    "gfx/Texture.cpp"
    "gfx/Texture.hpp"
    "gfx/VertexArray.cpp"
    "gfx/VertexArray.hpp"
    "gfx/VertexBuffer.cpp"
    "gfx/VertexBuffer.hpp"
    "gfx/VertexLayout.cpp"
    "gfx/VertexLayout.hpp"
    "gfx/Window.cpp"
    "gfx/Window.hpp"

    "world/Block.hpp"
    "world/BlockMesh.cpp"
    "world/BlockMesh.hpp"
    "world/Chunk.cpp"
    "world/Chunk.hpp"
    "world/ChunkMesh.cpp"
    "world/ChunkMesh.hpp"
    "world/Generator.cpp"
    "world/Generator.hpp"
    "world/Player.cpp"
    "world/Player.hpp"
    "world/World.cpp"
    "world/World.hpp"

    "general/Direction.hpp"
    "general/Ray.cpp"
    "general/Ray.hpp"
    "general/db_perlin.cpp"
    "general/stb_image.cpp"
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
