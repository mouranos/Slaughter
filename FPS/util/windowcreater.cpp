#include "windowcreater.h"
#include <stdexcept>

Window::Window(unsigned int width, unsigned int height, std::string title)
{
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(!window_)
    {
        throw std::runtime_error("Failed to create GLFW Window");
    }

    glfwMakeContextCurrent(window_);

    if(glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(window_);
        throw std::runtime_error("Failed to Initialize GLEW");
    }
}

Window::~Window()
{
    glfwDestroyWindow(window_);
}

GLFWOneTimeInit::GLFWOneTimeInit()
{
    if(!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwSetErrorCallback([](int error, const char* description)
    {
        throw std::runtime_error(description);
    });
}

GLFWOneTimeInit::~GLFWOneTimeInit()
{
    glfwTerminate();
}
