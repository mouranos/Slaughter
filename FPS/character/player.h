#ifndef PLAYER_H
#define PLAYER_H

#include <GLFW/glfw3.h>
#include "character.h"
#include "util/bulletgenobj.h"

class Player : public Character{
public:
    Player();
    void control(GLFWwindow* window,btVector3 direction,btScalar deltaTime, std::list<DynamicsWorld::ObjectData>::iterator itr);
};

#endif
