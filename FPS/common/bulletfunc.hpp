#ifndef BULLETFUNC_H
#define BULLETFUNC_H

extern btDynamicsWorld* dynamicsWorld;
extern btRigidBody* groundBody;

void initializer();
void initWorld();
void initObject();
void display();
void deleter();

#endif
