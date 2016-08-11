#include "bulletgenobj.h"
#include <btBulletDynamicsCommon.h>
#include <GL/glew.h>
#include <stdexcept>

DynamicsWorld::DynamicsWorld()
    : config(), dispatcher(&config), broadphase(), solver(),
      dynamicsWorld(&dispatcher, &broadphase, &solver, &config)
{
    dynamicsWorld.setGravity(btVector3(0, -9.8, 0));

    std::unique_ptr<btCollisionShape> collisionShape(
        std::make_unique<btStaticPlaneShape>(btVector3(0.f,1.f,0.f),0.f));

    std::unique_ptr<btDefaultMotionState> motionState;

    rigidBodies.push_back(
        {{0.0, motionState.get(), collisionShape.get()}, std::move(motionState), std::move(collisionShape)});

    groundBody = &rigidBodies.back().body;

    groundBody->setRestitution(0.f);
    groundBody->setFriction(0.8f);

    dynamicsWorld.addRigidBody(groundBody);
}

std::list<DynamicsWorld::ObjectData>::iterator
DynamicsWorld::addRigidBody(SHAPE shape, btVector3 halfExtents, btScalar mass,
                            btVector3 inertia, btVector3 pos, btVector3 axis,
                            btScalar angle, btScalar restitution,
                            btScalar friction)
{
    std::unique_ptr<btCollisionShape> collisionShape;
    if(shape == CUBE)
    {
        collisionShape = std::make_unique<btBoxShape>(halfExtents);
    }
    else if(shape == SPHERE)
    {
        collisionShape = std::make_unique<btSphereShape>(halfExtents.x());
    }
    else if(shape == CAPSULE)
    {
        collisionShape = std::make_unique<btCapsuleShape>(halfExtents.x(),halfExtents.y());
    }
    else
    {
        throw std::runtime_error(
            "I only support two shapes. Choose CUBE or SPHERE.");
    }

    btQuaternion qrot(axis, angle);
//    btDefaultMotionState motionState(btTransform(qrot, pos));
    std::unique_ptr<btMotionState> motionState(std::make_unique<btDefaultMotionState>(btTransform(qrot, pos)));
    collisionShape->calculateLocalInertia(mass, inertia);
    rigidBodies.push_back({{mass, motionState.get(), collisionShape.get(), inertia}, std::move(motionState),
                           std::move(collisionShape)});

    btRigidBody& rigidBody = rigidBodies.back().body;

    rigidBody.setRestitution(restitution);
    rigidBody.setFriction(friction);
    rigidBody.setLinearVelocity(btVector3(0.f,0.f,0.f));
    dynamicsWorld.addRigidBody(&rigidBody);
    return std::prev(rigidBodies.end());
}

void DynamicsWorld::removeRigidBody(
    std::list<DynamicsWorld::ObjectData>::iterator it)
{
    dynamicsWorld.removeRigidBody(&it->body);
    rigidBodies.erase(it);
}

DynamicsWorld::~DynamicsWorld()
{
    for(auto& i : rigidBodies)
    {
        dynamicsWorld.removeRigidBody(&i.body);
    }
}
