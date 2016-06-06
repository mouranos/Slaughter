#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "windowCreater.h"

Window::Window(unsigned int iwidth, unsigned int iheight, const char* ititle)
{
    Window::width = iwidth;
    Window::height = iheight;
    Window::title = ititle;
}

bool Window::windowInit()
{

    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);

    Window::window = glfwCreateWindow(Window::width, Window::height, Window::title, NULL, NULL);
    glfwMakeContextCurrent(window);

    if(window==NULL){
        fprintf(stderr, "Failed to open GLFW\n");
        glfwTerminate();
        return false;
    }

    if(glewInit()!=GLEW_OK){
        fprintf(stderr, "Failed to Initialize GLEW\n");
        glfwTerminate();
        return false;
    }
    return true;
}

GLFWwindow* Window::getWindowptr()
{
    return window;
}
