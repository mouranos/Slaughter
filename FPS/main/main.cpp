#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

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
    //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glm::vec3 initialPos = glm::vec3(0.0f, 0.0f, 0.0f);
    ComputeMatrices worldMatrices(window, initialPos);

    // Initialize dynamics world
    DynamicsWorld dynamicsWorld;

    // World's shader
    GLuint worldProgramID = LoadShaders("shader/mainVertexShader.glsl",
                                        "shader/mainFragmentShader.glsl");

    // World's matrices
    GLuint worldProjectionMatrixID = glGetUniformLocation(worldProgramID, "P");
    GLuint worldViewMatrixID = glGetUniformLocation(worldProgramID, "V");

    // Ground's matrices
    GLuint groundModelMatrixID = glGetUniformLocation(worldProgramID, "M");

    // Ground's IDs
    GLuint groundVertexPositionModelspaceID =
        glGetAttribLocation(worldProgramID, "vertexPosition_modelspace");
    GLuint groundVertexUVID = glGetAttribLocation(worldProgramID, "vertexUV");
    GLuint groundTextureID =
        glGetUniformLocation(worldProgramID, "myTextureSamplar");

    // Ground's data
    float halfGroundSize = 100.0f;
    const GLfloat groundVertexBufferData[] = {
        halfGroundSize,  -2.0f,           halfGroundSize,  halfGroundSize,
        -2.0f,           -halfGroundSize, -halfGroundSize, -2.0f,
        -halfGroundSize, -halfGroundSize, -2.0f,           halfGroundSize};

    const GLfloat groundUvBufferData[] = {0.0f, 0.0f, 1.0f, 0.0f,
                                          1.0f, 1.0f, 0.0f, 1.0f};

    GLuint groundTexture =
        loadBMP_custom("material/building/ground/groundTexture.bmp");

    // Ground's buffers
    GLuint groundVertexBuffer;
    glGenBuffers(1, &groundVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, groundVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertexBufferData),
                 groundVertexBufferData, GL_STATIC_DRAW);

    GLuint groundUvBuffer;
    glGenBuffers(1, &groundUvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, groundUvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundUvBufferData),
                 groundUvBufferData, GL_STATIC_DRAW);

    // m16's matrices
    GLuint m16ProjectionMatrixID = glGetUniformLocation(worldProgramID, "P");
    GLuint m16ViewMatrixID = glGetUniformLocation(worldProgramID, "V");
    GLuint m16ModelMatrixID = glGetUniformLocation(worldProgramID, "M");

    // m16's MVP
    glm::mat4 m16ProjectionMatrix =
        glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f); // TODO: set cliping
    glm::mat4 m16ViewMatrix = glm::lookAt(
        glm::vec3(30, 10, 10), glm::vec3(-20, 0, 10), glm::vec3(0, 1, 0));
    glm::mat4 m16ModelMatrix = glm::mat4(1.0f);

    // m16's IDs
    GLuint m16VertexPositionModelspaceID =
        glGetAttribLocation(worldProgramID, "vertexPosition_modelspace");
    GLuint m16VertexUVID = glGetAttribLocation(worldProgramID, "vertexUV");
    GLuint m16TextureID =
        glGetUniformLocation(worldProgramID, "myTextureSamplar");

    // m16's data
    loadOBJ m16obj("material/gun/m16/m16.obj");
    GLuint m16Texture = loadBMP_custom("material/gun/m16/m16.bmp");

    // m16's buffers
    GLuint m16VertexBuffer;
    glGenBuffers(1, &m16VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m16VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 m16obj.getIndexedVertices().size() * sizeof(glm::vec3),
                 &m16obj.getIndexedVertices()[0], GL_STATIC_DRAW);

    GLuint m16UvBuffer;
    glGenBuffers(1, &m16UvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m16UvBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 m16obj.getIndexedUvs().size() * sizeof(glm::vec2),
                 &m16obj.getIndexedUvs()[0], GL_STATIC_DRAW);

    GLuint m16ElementBuffer;
    glGenBuffers(1, &m16ElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m16ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 m16obj.getIndices().size() * sizeof(unsigned short),
                 &m16obj.getIndices()[0], GL_STATIC_DRAW);

    // Light ID
    GLuint LightID =
        glGetUniformLocation(worldProgramID, "LightPosition_worldspace");

    // Ball
    // Ball's data
    loadOBJ ballObj("material/shape/sphere/webtrcc.obj");
    GLuint ballTexture = loadBMP_custom("material/shape/sphere/earth.bmp");

    // Ball's IDs
    GLuint ballVertexPositionID =
        glGetAttribLocation(worldProgramID, "vertexPosition_modelspace");
    GLuint ballVertexUVID = glGetAttribLocation(worldProgramID, "vertexUV");
    GLuint ballTextureID =
        glGetAttribLocation(worldProgramID, "myTextureSamplar");

    // Ball's buffers
    GLuint ballVertexBuffer;
    glGenBuffers(1, &ballVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, ballVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 ballObj.getIndexedVertices().size() * sizeof(glm::vec3),
                 &ballObj.getIndexedVertices()[0], GL_STATIC_DRAW);

    GLuint ballUvBuffer;
    glGenBuffers(1, &ballUvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, ballUvBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 ballObj.getIndexedUvs().size() * sizeof(glm::vec2),
                 &ballObj.getIndexedUvs()[0], GL_STATIC_DRAW);

    GLuint ballNormalBuffer;
    glGenBuffers(1, &ballNormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, ballNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 ballObj.getIndexedNormals().size() * sizeof(glm::vec3),
                 &ballObj.getIndexedNormals()[0], GL_STATIC_DRAW);

    GLuint ballElementBuffer;
    glGenBuffers(1, &ballElementBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, ballElementBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 ballObj.getIndices().size() * sizeof(unsigned short),
                 &ballObj.getIndices()[0], GL_STATIC_DRAW);

    btRigidBody* ball;
    dynamicsWorld.addRigidBody(
        ball, SPHERE, btVector3(1.0f, 0.0f, 0.0f), 15.0f,
        btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 15.0f, 0.0f),
        btVector3(0.0f, 1.0f, 0.0f), M_PI * (0.0f / 180.0f), 0.8f, 0.7f);

    GLuint ballModelMatrixID = glGetUniformLocation(worldProgramID, "M");

    double start = glfwGetTime();
    do
    {
        double deltaTime = getDeltaTime(&start);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Draw world(ground,ball)
        glUseProgram(worldProgramID);

        worldMatrices.computeMatricesFromInputs(deltaTime);

        glm::mat4 worldProjectionMatrix = worldMatrices.getProjectionMatrix();
        glm::mat4 worldViewMatrix = worldMatrices.getViewMatrix();

        glm::mat4 groundModelMatrix = glm::mat4(1.0);

        glUniformMatrix4fv(worldProjectionMatrixID, 1, GL_FALSE,
                           &worldProjectionMatrix[0][0]);
        glUniformMatrix4fv(worldViewMatrixID, 1, GL_FALSE,
                           &worldViewMatrix[0][0]);
        glUniformMatrix4fv(groundModelMatrixID, 1, GL_FALSE,
                           &groundModelMatrix[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, groundTexture);

        glUniform1i(groundTextureID, 0);

        glEnableVertexAttribArray(groundVertexPositionModelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, groundVertexBuffer);
        glVertexAttribPointer(groundVertexPositionModelspaceID, 3, GL_FLOAT,
                              GL_FALSE, 0, static_cast<GLvoid*>(0));

        glEnableVertexAttribArray(groundVertexUVID);
        glBindBuffer(GL_ARRAY_BUFFER, groundUvBuffer);
        glVertexAttribPointer(groundVertexUVID, 2, GL_FLOAT, GL_FALSE, 0,
                              static_cast<GLvoid*>(0));

        glDrawArrays(GL_QUADS, 0, 3 * 4);

        glDisableVertexAttribArray(groundVertexPositionModelspaceID);
        glDisableVertexAttribArray(groundVertexUVID);

        // Draw ball
        glm::vec3 ballPos(ball->getCenterOfMassPosition().x(),
                          ball->getCenterOfMassPosition().y(),
                          ball->getCenterOfMassPosition().z());
        float ballAngle = ball->getOrientation().getAngle();
        glm::vec3 ballAxis(ball->getOrientation().getAxis().x(),
                           ball->getOrientation().getAxis().y(),
                           ball->getOrientation().getAxis().z());
        glm::mat4 ballTranslationMatrix = glm::translate(ballPos);
        glm::mat4 ballRotationMatrix = glm::rotate(ballAngle, ballAxis);
        glm::mat4 ballScalingMatrix = glm::scale(glm::vec3(2, 2, 2));
        glm::mat4 ballModelMatrix = ballTranslationMatrix * ballRotationMatrix *
                                    ballScalingMatrix * glm::mat4(1.0f);

        glUniformMatrix4fv(ballModelMatrixID, 1, GL_FALSE,
                           &ballModelMatrix[0][0]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ballTexture);

        glUniform1i(ballTextureID, 0);

        glEnableVertexAttribArray(ballVertexPositionID);
        glBindBuffer(GL_ARRAY_BUFFER, ballVertexBuffer);
        glVertexAttribPointer(ballVertexPositionID, 3, GL_FLOAT, GL_FALSE, 0,
                              static_cast<GLvoid*>(0));

        glEnableVertexAttribArray(ballVertexUVID);
        glBindBuffer(GL_ARRAY_BUFFER, ballUvBuffer);
        glVertexAttribPointer(ballVertexUVID, 2, GL_FLOAT, GL_FALSE, 0,
                              static_cast<GLvoid*>(0));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ballElementBuffer);
        glDrawElements(GL_TRIANGLES, ballObj.getIndices().size(),
                       GL_UNSIGNED_SHORT, nullptr);

        // Light
        glm::vec3 lightPos = glm::vec3(4, 4, 4);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        // Draw gun
        glUniformMatrix4fv(m16ProjectionMatrixID, 1, GL_FALSE,
                           &m16ProjectionMatrix[0][0]);
        glUniformMatrix4fv(m16ViewMatrixID, 1, GL_FALSE, &m16ViewMatrix[0][0]);
        glUniformMatrix4fv(m16ModelMatrixID, 1, GL_FALSE,
                           &m16ModelMatrix[0][0]);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m16Texture);

        glUniform1i(m16TextureID, 0);

        glEnableVertexAttribArray(m16VertexPositionModelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, m16VertexBuffer);
        glVertexAttribPointer(m16VertexPositionModelspaceID, 3, GL_FLOAT,
                              GL_FALSE, 0, static_cast<GLvoid*>(0));

        glEnableVertexAttribArray(m16VertexUVID);
        glBindBuffer(GL_ARRAY_BUFFER, m16UvBuffer);
        glVertexAttribPointer(m16VertexUVID, 2, GL_FLOAT, GL_FALSE, 0,
                              static_cast<GLvoid*>(0));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m16ElementBuffer);
        glDrawElements(GL_TRIANGLES, m16obj.getIndices().size(),
                       GL_UNSIGNED_SHORT, nullptr);

        glDisableVertexAttribArray(m16VertexPositionModelspaceID);
        glDisableVertexAttribArray(m16VertexUVID);

        glDisable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);

    glDeleteBuffers(1, &m16VertexBuffer);
    glDeleteBuffers(1, &m16UvBuffer);
    glDeleteBuffers(1, &m16ElementBuffer);
    glDeleteBuffers(1, &groundVertexBuffer);
    glDeleteBuffers(1, &groundVertexUVID);
    glDeleteProgram(worldProgramID);
    glDeleteTextures(1, &m16Texture);
    glDeleteTextures(1, &groundTexture);

    return true;
}

double getDeltaTime(double* lastTime){
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - *lastTime;
    *lastTime = currentTime;
    return deltaTime;
}
