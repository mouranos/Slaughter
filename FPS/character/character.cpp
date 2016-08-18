#include "character.h"

// class Chatacter

Character::Character(DynamicsWorld::ObjectData& characterData, int hp,
                     int maxSpeed, int power)
    : characterData_(characterData), hp_(std::move(hp)),
      maxSpeed_(std::move(maxSpeed)), power_(std::move(power))
{
}

int Character::getHp()
{
    return hp_;
}

void Character::setHp(int hp){
    hp_ = hp;
}

int Character::getPower()
{
    return power_;
}

void Character::setPower(int power)
{
    power_ = power;
}

int Character::getMaxSpeed()
{
    return maxSpeed_;
}

void Character::setMaxSpeed(int maxSpeed)
{
    maxSpeed_ = maxSpeed;
}

DynamicsWorld::ObjectData& Character::getObjectData()
{
    return characterData_;
}

void Character::moveTowardTarget(Character target)
{
    getObjectData().body.setLinearVelocity((target.getObjectData().body.getCenterOfMassPosition() - getObjectData().body.getCenterOfMassPosition()).normalize() * getMaxSpeed());
}
