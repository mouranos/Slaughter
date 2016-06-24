#ifndef WINDOWCREATER_H
#define WINDOWCREATER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
    Window(unsigned int width, unsigned int height, std::string title);
    ~Window();
    operator GLFWwindow*() { return window_;}

private:
    GLFWwindow* window_;
};

#endif
