# Add sources to the project's executable
add_executable (MinecraftClone
	"main.cpp"
	"State.hpp"
)

# Add subdirectories
add_subdirectory(gfx)
add_subdirectory(general)
add_subdirectory(world)

# Link the executable with the libraries
target_link_libraries(MinecraftClone PRIVATE
	glfw ${GLFW_LIBRARIES}
	glad ${CMAKE_DL_LIBS}
	glm
	stb-image
)

# Configure GLFW
target_include_directories(MinecraftClone PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(MinecraftClone PRIVATE "GLFW_INCLUDE_NONE")

# Copy resources to the build directory
add_custom_command(
    TARGET MinecraftClone PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/res/" $<TARGET_FILE_DIR:MinecraftClone>/res
)
