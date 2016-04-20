#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <common/windowCreater.hpp>
#include "title.hpp"

extern GLFWwindow* window;

void title(){
    windowInit(1440,900,"開成祭 FPS-TITLE");
}
