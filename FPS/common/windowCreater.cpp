#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "windowCreater.hpp"

extern GLFWwindow* window;

bool windowInit(int windowWidth, int windowHeight,const char* title){
    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }
    glfwWindowHint(GLFW_SAMPLES,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);

    window = glfwCreateWindow(windowWidth,windowHeight,title,NULL,NULL);
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
