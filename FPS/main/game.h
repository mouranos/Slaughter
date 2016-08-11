#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

bool startGame();
double getDeltaTime(double* lastTime);
glm::mat4 gunViewMatrixHandler(GLFWwindow* window);
void fire();
#endif
