#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "character.h"

class Enemy : public Character{
public:
    Enemy();
    btVector3 interceptorDirection(btVector3 enemyPos, btVector3 enemyVelocity, btVector3 playerPos, btVector3 playerVelocity);
};

#endif
