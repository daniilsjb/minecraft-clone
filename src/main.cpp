#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

static void OnGlfwError(int error, const char* description);
static void OnResize(GLFWwindow* window, int width, int height);

static void APIENTRY OnOpenGLDebug(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

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
        std::cout << "ERRRO::GLAD::LoadGLLoader: Could not initialize GLAD\n";
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
        glDebugMessageCallback(OnOpenGLDebug, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.5f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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
    std::cerr << "GLFW::ERROR::";
    switch (error)
    {
        case GLFW_NOT_INITIALIZED: std::cerr << "NOT_INITIALIZED"; break;
        case GLFW_NO_CURRENT_CONTEXT: std::cerr << "NO_CURRENT_CONTEXT"; break;
        case GLFW_INVALID_ENUM: std::cerr << "INVALID_ENUM"; break;
        case GLFW_INVALID_VALUE: std::cerr << "INVALID_VALUE"; break;
        case GLFW_OUT_OF_MEMORY: std::cerr << "OUT_OF_MEMORY"; break;
        case GLFW_API_UNAVAILABLE: std::cerr << "API_UNAVAILABLE"; break;
        case GLFW_VERSION_UNAVAILABLE: std::cerr << "VERSION_UNAVAILABLE"; break;
        case GLFW_PLATFORM_ERROR: std::cerr << "PLATFORM_ERROR"; break;
        case GLFW_FORMAT_UNAVAILABLE: std::cerr << "FORMAT_UNAVAILABLE"; break;
        default: std::cerr << "UNKNOWN"; break;
    }

    std::cerr << ": " << description << '\n';
}

void OnOpenGLDebug(
    [[maybe_unused]] GLenum source,
    [[maybe_unused]] GLenum type,
    [[maybe_unused]] unsigned int id,
    [[maybe_unused]] GLenum severity,
    [[maybe_unused]] GLsizei length,
    [[maybe_unused]] const char* message,
    [[maybe_unused]] const void* userParam)
{
    std::cerr << "OPENGL::DEBUG::";
    switch (source)
    {
        case GL_DEBUG_SOURCE_API: std::cerr << "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cerr << "WINDOW_SYSTEM"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "SHADER_COMPILER"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: std::cerr << "THIRD_PARTY"; break;
        case GL_DEBUG_SOURCE_APPLICATION: std::cerr << "APPLICATION"; break;
        case GL_DEBUG_SOURCE_OTHER: std::cerr << "OTHER_SOURCE"; break;
        default: std::cerr << "UNKNOWN_SOURCE"; break;
    }

    std::cerr << "::";
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR: std::cerr << "ERROR"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "DEPRECATED_BEHAVIOR"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cerr << "UNDEFINED_BEHAVIOR"; break;
        case GL_DEBUG_TYPE_PORTABILITY: std::cerr << "PORTABILITY"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: std::cerr << "PERFORMANCE"; break;
        case GL_DEBUG_TYPE_MARKER: std::cerr << "MARKER"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP: std::cerr << "PUSH_GROUP"; break;
        case GL_DEBUG_TYPE_POP_GROUP: std::cerr << "POP_GROUP"; break;
        case GL_DEBUG_TYPE_OTHER: std::cerr << "OTHER_TYPE"; break;
        default: std::cerr << "UNKNOWN_TYPE"; break;
    }

    std::cerr << "::";
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH: std::cerr << "HIGH"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: std::cerr << "MEDIUM"; break;
        case GL_DEBUG_SEVERITY_LOW: std::cerr << "LOW"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "NOTIFICATION"; break;
        default: std::cerr << "UNKNOWN_SEVERITY"; break;
    }

    std::cerr << ": " << message << '\n';
}

void OnResize(
    [[maybe_unused]] GLFWwindow* window,
    [[maybe_unused]] int width,
    [[maybe_unused]] int height)
{
    glViewport(0, 0, width, height);
}
