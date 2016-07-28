#include <iostream>
#include <math.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "controls.h"

ComputeMatrices::ComputeMatrices(GLFWwindow* window, glm::vec3 initialPos)
    : window_(std::move(window))
    , position_(std::move(initialPos))
    , horizontalAngle_(0.0f)
    , verticalAngle_(0.0f)
    , initialFov_(45.0f)
    , speed_(30.0f)
    , mouseSpeed_(0.0025f)
{}

glm::mat4 ComputeMatrices::getViewMatrix()
{
    return viewMatrix_;
}
glm::mat4 ComputeMatrices::getProjectionMatrix()
{
    return projectionMatrix_;
}

void ComputeMatrices::computeMatricesFromInputs(float deltaTime)
{
    double xMousePos, yMousePos;
    int windowWidth, windowHeight;
    glfwGetWindowSize(window_, &windowWidth, &windowHeight);
    glfwGetCursorPos(window_, &xMousePos, &yMousePos);
    glfwSetCursorPos(window_, windowWidth / 2, windowHeight / 2);
    horizontalAngle_ += mouseSpeed_ * float(windowWidth / 2 - xMousePos);
    verticalAngle_ += mouseSpeed_ * float(windowHeight / 2 - yMousePos);
    if(verticalAngle_ > M_PI/2){
        verticalAngle_ = M_PI/2;
    }else if(verticalAngle_ < -M_PI/2){
        verticalAngle_ = -M_PI/2;
    }
    glm::vec3 direction(cos(verticalAngle_) * sin(horizontalAngle_),
                        sin(verticalAngle_),
                        cos(verticalAngle_) * cos(horizontalAngle_));
    glm::vec3 right = glm::vec3(sin(horizontalAngle_ - 3.14f / 2.0f), 0,
                                cos(horizontalAngle_ - 3.14f / 2.0f));
    glm::vec3 up = glm::cross(right, direction);
    if(glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
    {
        if(glfwGetKey(window_, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
            position_ += glm::vec3(sin(horizontalAngle_), 0, cos(horizontalAngle_)) *
                        deltaTime * (speed_ * 2);
        }else{
            position_ += glm::vec3(sin(horizontalAngle_), 0, cos(horizontalAngle_)) *
                deltaTime * speed_;
        }
    }
    if(glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
    {
        position_ -= glm::vec3(sin(horizontalAngle_), 0, cos(horizontalAngle_)) *
                    deltaTime * speed_;
    }
    if(glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
    {
        position_ += right * deltaTime * speed_;
    }
    if(glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
    {
        position_ -= right * deltaTime * speed_;
    }
    if(glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        position_ += glm::vec3(0, 1, 0) * deltaTime * speed_;
    }
    if(glfwGetKey(window_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        position_ -= glm::vec3(0, 1, 0) * deltaTime * speed_;
    }

    float FoV = initialFov_;
    projectionMatrix_ = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 1000.0f);
    viewMatrix_ = glm::lookAt(position_, position_ + direction, up);
}
