#include <btBulletDynamicsCommon.h>
#include <util/objloader.h>
#include "player.h"

Player::Player()
    : Character(100,10000,20,btVector3(10,20,10),60,btVector3(0,20,0))
{}
