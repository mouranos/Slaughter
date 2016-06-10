#include "windowcreater.h"
#include <iostream>
#include <stdexcept>

Window::Window(unsigned int width, unsigned int height, std::string title)
{
    if(!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    glfwSetErrorCallback([](int error, const char* description)
    {
        throw std::runtime_error(description);
    });

    window_ = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    glfwMakeContextCurrent(window_);

    if(glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to Initialize GLEW\n");
        glfwTerminate();
    }
}
