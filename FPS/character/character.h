#ifndef CHARACTER_H
#define CHARACTER_H
#include <btBulletDynamicsCommon.h>
#include <stdio.h>

class Character
{
    struct CharacterBody
    {
        CharacterBody(btVector3 halfExtents, btScalar mass, btVector3 pos);
        btVector3 halfExtents_;
        btScalar mass_;
        btVector3 inertia_;
        btVector3 pos_;
        btVector3 axis_;
        btScalar angle_;
        btScalar restitution_;
        btScalar friction_;
    };

public:
    Character(int hp, int speed, int power, btVector3 halfExtents,
              btScalar mass, btVector3 pos);
    int getHp();
    int getPower();
    int getSpeed();

protected:
    int hp_;
    int speed_;
    int power_;
    CharacterBody entity_;
};

#endif
