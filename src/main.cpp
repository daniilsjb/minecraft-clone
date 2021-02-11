#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>

#include "Game.hpp"

const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

static void OnGlfwError(int error, const char* description);
static void OnResize(GLFWwindow* window, int width, int height);

static void APIENTRY OnOpenGLLog(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

int main()
{
    glfwSetErrorCallback(OnGlfwError);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);  
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minecraft Clone", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, OnResize);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "[GLAD] Could not initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    GLint flags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(GL_CONTEXT_FLAG_DEBUG_BIT))
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OnOpenGLLog, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // stbi_set_flip_vertically_on_load(true);

    Game game;
    game.Start();

    double deltaTime = glfwGetTime();
    double lastFrame = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        game.Update(deltaTime);
        game.Render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

void OnGlfwError(
    [[maybe_unused]] int error,
    [[maybe_unused]] const char* description)
{
    std::cerr << "[GLFW] " << description << '\n';

    std::cerr << "-- Type: ";
    switch (error)
    {
        case GLFW_NOT_INITIALIZED: std::cerr << "Not Initialized"; break;
        case GLFW_NO_CURRENT_CONTEXT: std::cerr << "No Current Context"; break;
        case GLFW_INVALID_ENUM: std::cerr << "Invalid Enum"; break;
        case GLFW_INVALID_VALUE: std::cerr << "Invalid Value"; break;
        case GLFW_OUT_OF_MEMORY: std::cerr << "Out of Memory"; break;
        case GLFW_API_UNAVAILABLE: std::cerr << "API Unavailable"; break;
        case GLFW_VERSION_UNAVAILABLE: std::cerr << "Version Unavailable"; break;
        case GLFW_PLATFORM_ERROR: std::cerr << "Platform Error"; break;
        case GLFW_FORMAT_UNAVAILABLE: std::cerr << "Format Unavailable"; break;
        default: std::cerr << "Unknown"; break;
    }
    std::cerr << '\n';
}

void OnOpenGLLog(
    [[maybe_unused]] GLenum source,
    [[maybe_unused]] GLenum type,
    [[maybe_unused]] unsigned int id,
    [[maybe_unused]] GLenum severity,
    [[maybe_unused]] GLsizei length,
    [[maybe_unused]] const char* message,
    [[maybe_unused]] const void* userParam)
{
    std::cerr << "[OpenGL] " << message << '\n';

    std::cerr << "-- Source: ";
    switch (source)
    {
        case GL_DEBUG_SOURCE_API: std::cerr << "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cerr << "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: std::cerr << "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION: std::cerr << "Application"; break;
        case GL_DEBUG_SOURCE_OTHER: std::cerr << "Other"; break;
        default: std::cerr << "Unknown"; break;
    }
    std::cerr << '\n';

    std::cerr << "-- Type: ";
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR: std::cerr << "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cerr << "Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY: std::cerr << "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: std::cerr << "Performance"; break;
        case GL_DEBUG_TYPE_MARKER: std::cerr << "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP: std::cerr << "Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP: std::cerr << "Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER: std::cerr << "Other"; break;
        default: std::cerr << "Unknown"; break;
    }
    std::cerr << '\n';

    std::cerr << "-- Severity: ";
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH: std::cerr << "High"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: std::cerr << "Medium"; break;
        case GL_DEBUG_SEVERITY_LOW: std::cerr << "Low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "Notifaction"; break;
        default: std::cerr << "Unknown"; break;
    }
    std::cerr << '\n';
}

void OnResize(
    [[maybe_unused]] GLFWwindow* window,
    [[maybe_unused]] int width,
    [[maybe_unused]] int height)
{
    glViewport(0, 0, width, height);
}
