#ifndef BULLETFUNC_H
#define BULLETFUNC_H

#include "btBulletDynamicsCommon.h"

constexpr double RADIAN = 180.0/3.1415;

class DynamicsWorld{
public:
    DynamicsWorld();
    ~DynamicsWorld();
    void initWorld();
    void initObject();
    void simulate();
    void display();
    void deleter();
private:
    btDynamicsWorld* dynamicsWorld;
    btRigidBody* groundBody;
};

#endif
