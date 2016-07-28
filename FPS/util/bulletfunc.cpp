#include "bulletfunc.h"
#include "btBulletDynamicsCommon.h"
#include <GL/glew.h>
#include <stdexcept>

DynamicsWorld::DynamicsWorld()
    : config(), dispatcher(&config), broadphase(), solver(),
      dynamicsWorld(&dispatcher, &broadphase, &solver, &config)
{
    dynamicsWorld.setGravity(btVector3(0, -10, 0));

    std::unique_ptr<btCollisionShape> collisionShape(
        std::make_unique<btBoxShape>(btVector3(100, -0.1, 100)));

    btVector3 pos(0, -0.05, 0);
    btVector3 axis(0, 1, 0);
    btScalar angle(btRadians(10.0f));
    btQuaternion qrot(axis, angle);
//    btDefaultMotionState motionState(btTransform(qrot, pos));
    std::unique_ptr<btMotionState> motionState(std::make_unique<btDefaultMotionState>(btTransform(qrot,pos)));

    rigidBodies.push_back(
        {{0.0, motionState.get(), collisionShape.get()}, std::move(collisionShape), std::move(motionState)});

    groundBody = &rigidBodies.back().body;

    btScalar restitution = btScalar(0.8);
    groundBody->setRestitution(restitution);

    dynamicsWorld.addRigidBody(groundBody);
}

std::vector<DynamicsWorld::ObjectData>::iterator
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
    else
    {
        throw std::runtime_error(
            "I only support two shapes. Choose CUBE or SPHERE.");
    }

    btQuaternion qrot(axis, angle);
//    btDefaultMotionState motionState(btTransform(qrot, pos));
    std::unique_ptr<btMotionState> motionState(std::make_unique<btDefaultMotionState>(btTransform(qrot, pos)));
    collisionShape->calculateLocalInertia(mass, inertia);
    rigidBodies.push_back({{mass, motionState.get(), collisionShape.get(), inertia},
                           std::move(collisionShape), std::move(motionState)});

    btRigidBody& rigidBody = rigidBodies.back().body;

    rigidBody.setRestitution(restitution);
    rigidBody.setFriction(friction);
    dynamicsWorld.addRigidBody(&rigidBody);
    return std::prev(rigidBodies.end());
}

void DynamicsWorld::display()
{
    GLfloat amb[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat dif[4] = {0.6f, 0.6f, 0.2f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);

    btVector3 pos = groundBody->getCenterOfMassPosition();
    btScalar rot(btRadians(groundBody->getOrientation().getAngle()));
    btVector3 axis = groundBody->getOrientation().getAxis();

    glTranslatef(pos[0], pos[1], pos[2]);
    glRotated(rot, axis[0], axis[1], axis[2]);
    btVector3 halfExtent =
        static_cast<const btBoxShape*>(groundBody->getCollisionShape())
            ->getHalfExtentsWithMargin();
    glScaled(2 * halfExtent[0], 2 * halfExtent[1], 2 * halfExtent[2]);
}

void DynamicsWorld::removeRigidBody(
    std::vector<DynamicsWorld::ObjectData>::iterator it)
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
