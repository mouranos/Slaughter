#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "util/bulletgenobj.h"
#include "character/player.h"

class ComputeMatrices{
public:
    ComputeMatrices(GLFWwindow* window, Player player,
                    std::list<DynamicsWorld::ObjectData>::iterator itr);
    void computeMatricesFromInputs(float deltaTime);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::vec3 getPosition();
private:
    GLFWwindow* window_;
    Player player_;
    std::list<DynamicsWorld::ObjectData>::iterator itr_;
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
