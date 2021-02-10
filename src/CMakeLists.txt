# Add sources to the project's executable
add_executable (MinecraftClone "main.cpp")

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
