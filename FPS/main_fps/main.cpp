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

GLFWwindow* window;

int main(){
    if(!glfwInit()){
        fprintf(stderr,"Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);



    window = glfwCreateWindow(1440,900,"文化祭 FPS",glfwGetPrimaryMonitor(),NULL);

    if(window==NULL){
        fprintf(stderr,"Failed to open GLFW\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        fprintf(stderr,"Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint programID = LoadShaders("mainVertexShader.cpp","mainFragmentShader.cpp");

    GLuint MatrixID = glGetUniformLocation(programID,"MVP");

    GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");

    glm::mat4 Projection = glm::perspective(
                45.0f,
                4.0f/3.0f,
                0.1f,
                100.0f
                );
    glm::mat4 View = glm::lookAt(
                glm::vec3(10,10,10),
                glm::vec3(0,0,0),
                glm::vec3(0,1,0));
    glm::mat4 Model = glm::mat4(1.0f);

    do{
        glClear( GL_COLOR_BUFFER_BIT );

        glfwSwapBuffers(window);
        glfwPollEvents();

    }while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    glfwTerminate();
    return 0;
}
