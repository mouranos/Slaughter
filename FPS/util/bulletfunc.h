#ifndef BULLETFUNC_H
#define BULLETFUNC_H

#include "btBulletDynamicsCommon.h"

constexpr double RADIAN = 180.0/3.1415;

enum SHAPE{
    CUBE,
    SPHERE
};

class DynamicsWorld{
public:
    DynamicsWorld();
    ~DynamicsWorld();
    void initObject();
    void simulate();
    void display();
    void addRigidBody(btRigidBody* rigidBody, enum SHAPE shape, btScalar halfExtent, btScalar mass, btVector3 inertia, btVector3 pos, btVector3 axis, btScalar angle, btScalar restitution, btScalar friction);
    btDynamicsWorld* getDynamicsWorld(){
        return dynamicsWorld;
    }

private:
    btDynamicsWorld* dynamicsWorld;
    btRigidBody* groundBody;
    btDefaultCollisionConfiguration config;
    btCollisionDispatcher dispatcher;
    btDbvtBroadphase broadphase;
    btSequentialImpulseConstraintSolver solver;
};

#endif
