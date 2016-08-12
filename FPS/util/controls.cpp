#include "controls.h"
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <math.h>

// TODO: fix keys.
ComputeMatrices::ComputeMatrices(
    GLFWwindow* window, Player player,
    std::list<DynamicsWorld::ObjectData>::iterator itr)
    : window_(window), player_(player), itr_(itr),
      position_(itr_->body.getCenterOfMassPosition().x(),
                itr_->body.getCenterOfMassPosition().y()*2,
                itr_->body.getCenterOfMassPosition().z()),
      horizontalAngle_(0.f), verticalAngle_(0.f), initialFov_(45.f),
      speed_(0.f), mouseSpeed_(0.0025f)
{
    int windowWidth, windowHeight;
    glfwGetWindowSize(window_, &windowWidth, &windowHeight);
    glfwSetCursorPos(window_, windowWidth / 2, windowHeight / 2);
}

glm::mat4 ComputeMatrices::getViewMatrix()
{
    return viewMatrix_;
}
glm::mat4 ComputeMatrices::getProjectionMatrix()
{
    return projectionMatrix_;
}

glm::vec3 ComputeMatrices::getPosition()
{
    return position_;
}

void ComputeMatrices::computeMatricesFromInputs(float deltaTime)
{
    glfwPollEvents();
    double xMousePos, yMousePos;
    int windowWidth, windowHeight;
    glfwGetWindowSize(window_, &windowWidth, &windowHeight);
    glfwGetCursorPos(window_, &xMousePos, &yMousePos);
    glfwSetCursorPos(window_, windowWidth / 2, windowHeight / 2);
    horizontalAngle_ += mouseSpeed_ * float(windowWidth / 2 - xMousePos);
    verticalAngle_ += mouseSpeed_ * float(windowHeight / 2 - yMousePos);
    if(verticalAngle_ > M_PI / 2)
    {
        verticalAngle_ = M_PI / 2;
    }
    else if(verticalAngle_ < -M_PI / 2)
    {
        verticalAngle_ = -M_PI / 2;
    }
    if(horizontalAngle_ > M_PI)
    {
        horizontalAngle_ -= M_PI * 2;
    }
    else if(horizontalAngle_ < -M_PI)
    {
        horizontalAngle_ += M_PI * 2;
    }
    glm::vec3 eyeDirection(cos(verticalAngle_) * sin(horizontalAngle_),
                      sin(verticalAngle_),
                      cos(verticalAngle_) * cos(horizontalAngle_));
    glm::vec3 right = glm::vec3(sin(horizontalAngle_ - 3.14f / 2.f), 0,
                                cos(horizontalAngle_ - 3.14f / 2.f));
    glm::vec3 up = glm::cross(right, eyeDirection);

    btVector3 forward(btVector3(eyeDirection.x,0,eyeDirection.z).normalize());
    right /= right.length();

    itr_->body.setAngularFactor(0.f);
    btVector3 linearVelocity(0, 0, 0);

    bool shiftIsPressed = false;
    if(glfwGetKey(window_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        player_.halveMaxSpeed();
        shiftIsPressed = true;
    }

    if(glfwGetKey(window_, GLFW_KEY_A) == GLFW_RELEASE &&
       glfwGetKey(window_, GLFW_KEY_W) == GLFW_RELEASE &&
       glfwGetKey(window_, GLFW_KEY_S) == GLFW_RELEASE &&
       glfwGetKey(window_, GLFW_KEY_D) == GLFW_RELEASE)
    {
        if(speed_ > 0)
        {
            speed_ -= 10000 * deltaTime;
        }
        else
        {
            speed_ = 0;
        }
        if(itr_->body.getLinearVelocity().length() != 0){
            linearVelocity = const_cast<btVector3&>(itr_->body.getLinearVelocity()).normalize();
            linearVelocity.setY(0);
        }
    }
    else
    {
        if(speed_ < player_.getMaxSpeed())
        {
            speed_ += 10000 * deltaTime;
        }
        else
        {
            speed_ = player_.getMaxSpeed();
        }
        if(glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
        {
            if(glfwGetKey(window_, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            {
                linearVelocity += btVector3(forward.x() * 2, 0, forward.z() * 2);
            }
            else
            {
                linearVelocity += btVector3(forward.x(), 0, forward.z());
            }
        }
        if(glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
        {
            linearVelocity += btVector3(-forward.x(), 0, -forward.z());
        }
        if(glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
        {
            linearVelocity += btVector3(right.x, 0, right.z);
        }
        if(glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
        {
            linearVelocity += btVector3(-right.x, 0, -right.z);
        }
    }
    if(glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if(itr_->body.getCenterOfMassPosition().y() <=
           player_.getCharBody().getHalfExtents().y())
        {
            btScalar magnitude =
                (btScalar(1.f) / itr_->body.getInvMass()) * btScalar(100.f);
            itr_->body.applyCentralImpulse(btVector3(0, 1000000, 0) * magnitude);

        }
    }
    itr_->body.setLinearVelocity(linearVelocity * speed_ * deltaTime);
    if(shiftIsPressed)
    {
        player_.doubleMaxSpeed();
        shiftIsPressed = false;
    }
    position_ = glm::vec3(itr_->body.getCenterOfMassPosition().x(),
                          itr_->body.getCenterOfMassPosition().y()*2,
                          itr_->body.getCenterOfMassPosition().z());
    projectionMatrix_ = glm::perspective(initialFov_, 4.f / 3.f, 0.1f, 1000.f);
    viewMatrix_ = glm::lookAt(position_, position_ + eyeDirection, up);
}
