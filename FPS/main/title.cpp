#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <util/shader.h>
#include <util/windowcreater.h>
#include <util/textrenderer.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "title.h"
#include <math.h>

constexpr unsigned int WIDTH = 1440;
constexpr unsigned int HEIGHT = 900;
constexpr unsigned int TEXT_SIZE = 64;

bool title()
{
    Window window(WIDTH, HEIGHT, "開成祭 FPS-TITLE");

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glClearColor(0.0f, 0.0f, 0.0f, 1.f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    RenderText instructionToStart(U"Enterを押してゲームを開始！",TEXT_SIZE);
    RenderText manipulation(U"移動は矢印キー、ジャンプはスペースキー",TEXT_SIZE);
    double lastTime = glfwGetTime();
    double x = 0, y = 0, angle = 0;
    do
    {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        angle += 360.0/3.0 * deltaTime;
        if(angle > 360){
            angle -= 360;
        }

//        instructionToStart.render(x + 1000 * sin(angle *M_PI/180.0),y + 500 * cos(angle * M_PI/180.0));

        bool isUnsignedx,isUnsignedy;
        if(0 > x){
            isUnsignedx  = true;
        }
        else if(x > 1440 * 2){
            isUnsignedx = false;
        }
        if(0 > y){
            isUnsignedy = true;
        }
        else if(y > 832 * 2){
            isUnsignedy = false;
        }
        if(isUnsignedx){
            x += 500 * deltaTime;
        }else{
            x -= 500 * deltaTime;
        }
        if(isUnsignedy){
            y += 500 * deltaTime;
        }else{
            y -= 500 * deltaTime;
        }
        instructionToStart.render(x,y);
        manipulation.render(x,y-100);
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while(glfwGetKey(window, GLFW_KEY_ENTER) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);
    int width,height;
    glfwGetWindowSize(window,&width,&height);

    return true;
}
