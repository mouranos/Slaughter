#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "main.h"
#include <util/bulletfunc.h>
#include <util/controls.h>
#include <util/objloader.h>
#include <util/shader.h>
#include <util/texture.h>
#include <util/vboindexer.h>
#include <util/windowcreater.h>

constexpr unsigned int WIDTH = 1440;
constexpr unsigned int HEIGHT = 900;

bool start_fps()
{
    Window window(WIDTH, HEIGHT, "開成祭 FPS-main");

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Initialize dynamics world
    // DynamicsWorld dynamicsWorld;

    // Load shaders
    GLuint worldProgramID = LoadShaders("shaders/mainVertexShader.glsl",
                                        "shaders/mainGroundColorShader.glsl");

    GLuint m16ProgramID = LoadShaders("shaders/mainVertexShader.glsl",
                                      "shaders/mainFragmentShader.glsl");

    // Gun's matrices
    GLuint m16MatrixID = glGetUniformLocation(m16ProgramID, "MVP");

    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

    glm::mat4 View = glm::lookAt(glm::vec3(30, 10, 10), glm::vec3(-20, 0, 10),
                                 glm::vec3(0, 1, 0));

    glm::mat4 Model = glm::mat4(1.0f);

    glm::mat4 m16MVP = Projection * View * Model;

    // Ground's matrices
    GLuint worldMatrixID = glGetUniformLocation(worldProgramID, "MVP");
    GLuint worldViewMatrixID = glGetUniformLocation(worldProgramID, "V");
    GLuint worldModelMatrixID = glGetUniformLocation(worldProgramID, "M");

    // Worldspace ID
    GLuint vertexPositionModelspaceID =
        glGetAttribLocation(worldProgramID, "vertexPosition_modelspace");
    GLuint vertexColorID = glGetAttribLocation(worldProgramID, "vertexColor");

    // m16 ID
    GLuint m16VertexPositionModelspaceID =
        glGetAttribLocation(m16ProgramID, "vertexPosition_modelspace");
    GLuint m16VertexUVID = glGetAttribLocation(m16ProgramID, "vertexUV");

    GLuint m16TextureID =
        glGetUniformLocation(m16ProgramID, "myTexturesamplar");

    // Light ID
    GLuint LightID =
        glGetUniformLocation(worldProgramID, "LightPosition_worldspace");

    // m16's data
    loadOBJ m16obj("materials/Guns/m16/m16.obj", 0);

    GLuint m16Texture = loadBMP_custom("materials/Guns/m16/m16.bmp");

    // m16's buffers
    GLuint m16Vertexbuffer;
    glGenBuffers(1, &m16Vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m16Vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, m16obj.vertices.size() * sizeof(glm::vec3),
                 &m16obj.vertices[0], GL_STATIC_DRAW);

    GLuint m16Uvbuffer;
    glGenBuffers(1, &m16Uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m16Uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, m16obj.uvs.size() * sizeof(glm::vec2),
                 &m16obj.uvs[0], GL_STATIC_DRAW);

    GLuint m16Elementbuffer;
    glGenBuffers(1, &m16Elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m16Elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 m16obj.indices.size() * sizeof(unsigned short),
                 &m16obj.indices[0], GL_STATIC_DRAW);

    // Ground's data
    float groundSize = 100.0f;
    const GLfloat groundVertexBufferData[] = {
        groundSize,  -2.0f, groundSize,  groundSize,  -2.0f, -groundSize,
        -groundSize, -2.0f, -groundSize, -groundSize, -2.0f, groundSize};

    const GLfloat groundColorBufferData[] = {
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
    // Ground's buffers
    GLuint groundVertexbuffer;
    glGenBuffers(1, &groundVertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, groundVertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertexBufferData),
                 groundVertexBufferData, GL_STATIC_DRAW);

    GLuint groundColorbuffer;
    glGenBuffers(1, &groundColorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, groundColorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundColorBufferData),
                 groundColorBufferData, GL_STATIC_DRAW);

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Draw ground
        glUseProgram(worldProgramID);

        computeMatricesFromInputs(window);
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 worldMVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        glUniformMatrix4fv(worldMatrixID, 1, GL_FALSE, &worldMVP[0][0]);
        glUniformMatrix4fv(worldModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(worldViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        glm::vec3 lightPos = glm::vec3(4, 4, 4);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        glEnableVertexAttribArray(vertexPositionModelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, groundVertexbuffer);
        glVertexAttribPointer(vertexPositionModelspaceID, 3, GL_FLOAT, GL_FALSE,
                              0, static_cast<GLvoid*>(0));

        glEnableVertexAttribArray(vertexColorID);
        glBindBuffer(GL_ARRAY_BUFFER, groundColorbuffer);
        glVertexAttribPointer(vertexColorID, 3, GL_FLOAT, GL_FALSE, 0,
                              static_cast<GLvoid*>(0));

        glDrawArrays(GL_QUADS, 0, 3 * 4);

        glDisableVertexAttribArray(vertexPositionModelspaceID);
        glDisableVertexAttribArray(vertexColorID);

        glDisable(GL_DEPTH_TEST);

        // Draw gun
        glUseProgram(m16ProgramID);

        glUniformMatrix4fv(m16MatrixID, 1, GL_FALSE, &m16MVP[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m16Texture);

        glUniform1i(m16TextureID, 0);

        glEnableVertexAttribArray(m16VertexPositionModelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, m16Vertexbuffer);
        glVertexAttribPointer(m16VertexPositionModelspaceID, 3, GL_FLOAT,
                              GL_FALSE, 0, static_cast<GLvoid*>(0));

        glEnableVertexAttribArray(m16VertexUVID);
        glBindBuffer(GL_ARRAY_BUFFER, m16Uvbuffer);
        glVertexAttribPointer(m16VertexUVID, 2, GL_FLOAT, GL_FALSE, 0,
                              static_cast<GLvoid*>(0));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m16Elementbuffer);

        glDrawElements(GL_TRIANGLES, m16obj.indices.size(), GL_UNSIGNED_SHORT,
                       nullptr);

        glDisableVertexAttribArray(m16VertexPositionModelspaceID);
        glDisableVertexAttribArray(m16VertexUVID);

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);

    glDeleteBuffers(1, &m16Vertexbuffer);
    glDeleteBuffers(1, &m16Uvbuffer);
    glDeleteBuffers(1, &m16Elementbuffer);
    glDeleteBuffers(1, &groundVertexbuffer);
    glDeleteBuffers(1, &groundColorbuffer);
    glDeleteProgram(worldProgramID);
    glDeleteProgram(m16ProgramID);
    glDeleteTextures(1, &m16Texture);
    return true;
}
