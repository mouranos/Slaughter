#ifndef BULLETFUNC_H
#define BULLETFUNC_H

#include <btBulletDynamicsCommon.h>
#include <memory>
#include <vector>

enum SHAPE
{
    CUBE,
    SPHERE
};

class DynamicsWorld
{
public:
    struct ObjectData
    {
        btRigidBody body;
        std::unique_ptr<btCollisionShape> colShape;
        std::unique_ptr<btMotionState> motionState;
    };

    DynamicsWorld();
    ~DynamicsWorld();
    void display();
    std::vector<ObjectData>::iterator
    addRigidBody(SHAPE shape, btVector3 halfExtents, btScalar mass,
                 btVector3 inertia, btVector3 pos, btVector3 axis,
                 btScalar angle, btScalar restitution, btScalar friction);
    void removeRigidBody(std::vector<ObjectData>::iterator it);
    btDynamicsWorld& getDynamicsWorld() { return dynamicsWorld; }

private:
    btDefaultCollisionConfiguration config;
    btCollisionDispatcher dispatcher;
    btDbvtBroadphase broadphase;
    btSequentialImpulseConstraintSolver solver;
    btDiscreteDynamicsWorld dynamicsWorld;
    btRigidBody* groundBody;
    std::vector<ObjectData> rigidBodies;
};

#endif
