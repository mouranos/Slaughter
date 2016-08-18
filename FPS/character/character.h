#ifndef CHARACTER_H
#define CHARACTER_H
#include <stdio.h>
#include <btBulletDynamicsCommon.h>
#include "util/bulletgenobj.h"


class Character
{
public:
    Character(DynamicsWorld::ObjectData& characterData, int hp, int speed, int power);
    int getHp();
    void setHp(int);
    int getPower();
    void setPower(int);
    int getMaxSpeed();
    void setMaxSpeed(int);
    DynamicsWorld::ObjectData& getObjectData();
    void moveTowardTarget(Character target);

private:
    int hp_;
    int maxSpeed_;
    int power_;
    DynamicsWorld::ObjectData& characterData_;
};

#endif
