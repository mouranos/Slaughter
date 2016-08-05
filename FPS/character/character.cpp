#include "character.h"

// class Chatacter

Character::Character(int hp, int maxSpeed, int power, btVector3 halfExtents,
                     btScalar mass, btVector3 pos)
    : hp_(std::move(hp)), maxSpeed_(std::move(maxSpeed)), power_(std::move(power)),
      charBody_(halfExtents, mass, pos)
{
}

int Character::getHp()
{
    return hp_;
}

int Character::getPower()
{
    return power_;
}

int Character::getMaxSpeed()
{
    return maxSpeed_;
}

void Character::halveMaxSpeed()
{
    maxSpeed_ /= 2;
}

void Character::doubleMaxSpeed(){
    maxSpeed_ *= 2;
}

btVector3 Character::getDirection()
{
    return direction_;
}


// struct CharacterBody

Character::CharacterBody::CharacterBody(btVector3 halfExtents, btScalar mass,
                                        btVector3 pos)
    : halfExtents_(std::move(halfExtents)), mass_(std::move(mass)),
      pos_(std::move(pos))
{}

Character::CharacterBody Character::getCharBody()
{
    return charBody_;
}

btVector3 Character::CharacterBody::getHalfExtents()
{
    return halfExtents_;
}

btScalar Character::CharacterBody::getMass()
{
    return mass_;
}

btVector3 Character::CharacterBody::getPosition()
{
    return pos_;
}
