#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class ComputeMatrices{
public:
    ComputeMatrices(GLFWwindow* window, glm::vec3 initialPos);
    void computeMatricesFromInputs(float deltaTime);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
private:
    GLFWwindow* window_;
    glm::mat4 viewMatrix_;
    glm::mat4 projectionMatrix_;
    glm::vec3 position_;
    float horizontalAngle_;
    float verticalAngle_;
    float initialFov_;
    float speed_;
    float mouseSpeed_;
};
#endif
