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

constexpr unsigned int WIDTH = 1440;
constexpr unsigned int HEIGHT = 900;
constexpr unsigned int TEXT_SIZE = 64;

bool title()
{
    Window window(WIDTH, HEIGHT, "開成祭 FPS-TITLE");

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glClearColor(0.21f, 0.24f, 0.29f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    RenderText text(U"Enterを押してください",TEXT_SIZE);

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        text.render(300,300);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while(glfwGetKey(window, GLFW_KEY_ENTER) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);

    glfwTerminate();
    return true;
}
