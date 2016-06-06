#ifndef WINDOWCREATER_H
#define WINDOWCREATER_H

class Window
{
private:
    GLFWwindow* window;
    unsigned int width;
    unsigned int height;
    const char* title;
public:
    Window(unsigned int width, unsigned int height, const char* title);
    bool windowInit();
    GLFWwindow* getWindowptr();
};

#endif
