#ifndef CHARACTER_H
#define CHARACTER_H
#include <btBulletDynamicsCommon.h>
#include <stdio.h>

class Character
{
public:
    struct CharacterBody
    {
    public:
        CharacterBody(btVector3 halfExtents, btScalar mass, btVector3 pos);
        btVector3 getHalfExtents();
        btScalar getMass();
        btVector3 getPosition();
    private:
        btVector3 halfExtents_;
        btScalar mass_;
        btVector3 pos_;
    };
    Character(int hp, int speed, int power, btVector3 halfExtents,
              btScalar mass, btVector3 pos);
    int getHp();
    int getPower();
    int getMaxSpeed();
    void halveMaxSpeed();
    void doubleMaxSpeed();
    btVector3 getDirection();
    CharacterBody getCharBody();


protected:
    int hp_;
    int maxSpeed_;
    int power_;
    CharacterBody charBody_;
    btScalar turnSpeed_;
    btVector3 direction_;
};

#endif
