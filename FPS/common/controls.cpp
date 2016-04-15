#include <GLFW/glfw3.h>
extern GLFWwindow *window;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include "controls.hpp"
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 getViewMatrix()
{
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix()
{
    return ProjectionMatrix;
}
glm::vec3 position    = glm::vec3(0, 0, 5);
float horizontalAngle = 3.14f;
float verticalAngle   = 0.0f;
float initialFov      = 45.0f;
float speed           = 3.0f;
float mouseSpeed = 0.0025f;
void computeMatricesFromInputs()
{
    static double lastTime = glfwGetTime();
    double currentTime     = glfwGetTime();
    float deltaTime        = float(currentTime - lastTime);
    double xpos, ypos;
    int xnpos, ynpos;
    glfwGetWindowSize(window, &xnpos, &ynpos);
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPos(window, xnpos / 2, ynpos / 2);
    horizontalAngle += mouseSpeed * float(xnpos / 2 - xpos);
    verticalAngle += mouseSpeed * float(ynpos / 2 - ypos);
    glm::vec3 direction(cos(verticalAngle) * sin(horizontalAngle),
                        cos(verticalAngle),
                        cos(verticalAngle) * cos(horizontalAngle));
//    glm::vec3 direction(cos(verticalAngle) * sin(horizontalAngle),
//                        sin(verticalAngle),
//                        cos(verticalAngle) * cos(horizontalAngle));
    glm::vec3 right =
        glm::vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f));
    glm::vec3 up = glm::cross(right, direction);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position += direction * deltaTime * speed;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position -= direction * deltaTime * speed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        position += right * deltaTime * speed;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        position -= right * deltaTime * speed;
    }
    float FoV        = initialFov;
    ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
    ViewMatrix       = glm::lookAt(position, position + direction, up);
    lastTime         = currentTime;
}
