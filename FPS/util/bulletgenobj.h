#ifndef BULLET_GEN_OBJ_H
#define BULLET_GEN_OBJ_H

#include <btBulletDynamicsCommon.h>
#include <memory>
#include <list>

enum SHAPE
{
    BOX,
    SPHERE,
    CAPSULE
};

class DynamicsWorld
{
public:
    struct ObjectData
    {
        btRigidBody body;
        std::unique_ptr<btMotionState> motionState;
        std::unique_ptr<btCollisionShape> colShape;
    };

    DynamicsWorld();
    ~DynamicsWorld();
    std::list<ObjectData>::iterator
    addRigidBody(SHAPE shape, btVector3 halfExtents, btScalar mass,
                 btVector3 inertia, btVector3 pos, btVector3 axis,
                 btScalar angle, btScalar restitution, btScalar friction);
    void removeRigidBody(std::list<ObjectData>::iterator it);
    btDynamicsWorld& getDynamicsWorld() { return dynamicsWorld; }

private:
    btDefaultCollisionConfiguration config;
    btCollisionDispatcher dispatcher;
    btDbvtBroadphase broadphase;
    btSequentialImpulseConstraintSolver solver;
    btDiscreteDynamicsWorld dynamicsWorld;
    btRigidBody* groundBody;
    std::list<ObjectData> rigidBodies;
};

#endif
