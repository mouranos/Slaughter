#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <util/shader.h>
#include <util/windowcreater.h>
#include <util/textrenderer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

    GLuint programID =
        LoadShaders("shaders/titleVertexShader.glsl",
                    "shaders/titleFragmentShader.glsl");

    GLuint vertexPosition_screenspaceID =
        glGetAttribLocation(programID, "vertexPosition_screenspace");
    GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");
    GLint TextureID = glGetUniformLocation(programID, "myTextureSampler");

    std::u32string str32 = U"Enterを押してください";

    RenderText text(str32,TEXT_SIZE, 300, 300);


    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        text.render(0,0, vertexPosition_screenspaceID, vertexUVID, TextureID);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while(glfwGetKey(window, GLFW_KEY_ENTER) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);

    glDeleteProgram(programID);


    glfwTerminate();
    return true;
}
