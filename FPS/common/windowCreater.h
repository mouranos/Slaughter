#ifndef WINDOWCREATER_H
#define WINDOWCREATER_H

class Window
{
private:
    GLFWwindow* window;
    bool error;
public:
    Window(int width, int height, const char* title);
    bool getError();
    GLFWwindow* getWindowptr();
};

#endif
