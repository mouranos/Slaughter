#ifndef CHARACTER_H
#define CHARACTER_H
#include <stdio.h>

class Character{
public:
    int getHp();
    int getPower();
    int getSpeed();
protected:
    int hp;
    int speed;
    int power;
};

#endif
