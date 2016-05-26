#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/windowCreater.hpp>
#include "main.hpp"

extern GLFWwindow* window;


bool start_fps(){
    if(!windowInit(1440,900,"開成祭 FPS"))
        return false;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    GLuint programID = LoadShaders("mainVertexShader.vertexshader","mainFragmentShader.fragmentshader");
    GLuint MatrixID = glGetUniformLocation(programID,"MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");


    GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
    GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");
    GLuint vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");

    GLuint m16Texture = loadBMP_custom("materials/Guns/m16/m16.bmp");

    GLuint TextureID = glGetUniformLocation(programID, "myTexturesamplar");

    loadOBJ m16obj("materials/Guns/m16/m16.obj",0);


    GLuint m16_vertexbuffer;
    glGenBuffers(1, &m16_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m16_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, m16obj.vertices.size() * sizeof(glm::vec3), &m16obj.vertices[0], GL_STATIC_DRAW);

    GLuint m16_uvbuffer;
    glGenBuffers(1, &m16_uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m16_uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, m16obj.uvs.size() * sizeof(glm::vec2), &m16obj.uvs[0], GL_STATIC_DRAW);

    GLuint m16_elementbuffer;
    glGenBuffers(1, &m16_elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m16_elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m16obj.indices.size() * sizeof(unsigned short), &m16obj.indices[0], GL_STATIC_DRAW);

    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    do{
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glUseProgram(programID);

        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        glm::vec3 lightPos = glm::vec3(4,4,4);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m16Texture);

        glUniform1i(TextureID, 0);

        glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, m16_vertexbuffer);
        glVertexAttribPointer(
            vertexPosition_modelspaceID,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        glEnableVertexAttribArray(vertexUVID);
        glBindBuffer(GL_ARRAY_BUFFER, m16_uvbuffer);
        glVertexAttribPointer(
            vertexUVID,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m16_elementbuffer);

        glDrawElements(
            GL_TRIANGLES,
            m16obj.indices.size(),
            GL_UNSIGNED_SHORT,
            (void*)0
        );

        glDisableVertexAttribArray(vertexPosition_modelspaceID);
        glDisableVertexAttribArray(vertexUVID);
        glDisableVertexAttribArray(vertexNormal_modelspaceID);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    glDeleteBuffers(1,&m16_vertexbuffer);
    glDeleteBuffers(1,&m16_uvbuffer);
    glDeleteBuffers(1,&m16_elementbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &m16Texture);

    glfwTerminate();

    return true;
}
