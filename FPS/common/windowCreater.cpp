#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "windowCreater.h"

Window::Window(int width, int height, const char* title)
{
    Window::error = false;

    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        error = true;
    }

    glfwWindowHint(GLFW_SAMPLES,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);

    Window::window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(window);

    if(window==NULL){
        fprintf(stderr, "Failed to open GLFW\n");
        glfwTerminate();
        error = true;
    }

    if(glewInit()!=GLEW_OK){
        fprintf(stderr, "Failed to Initialize GLEW\n");
        glfwTerminate();
        error = true;
    }
}

bool Window::getError()
{
    return error;
}

GLFWwindow* Window::getWindowptr()
{
    return window;
}
