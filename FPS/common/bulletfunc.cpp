#include "btBulletDynamicsCommon.h"
#include "bulletfunc.hpp"

void initWorld(){
    btDefaultCollisionConfiguration* config = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(config);
    btDbvtBroadphase* broadphase = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
    dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
}

void initObject(){
    double radian;
    btCollisionShape* collision_shape = new btBoxShape(btVector3(3, 0.1, 3));

    btVector3 pos = btVector3(0, -2, 0);
    btVector3 axis = btVector3(0, 0, 1);
    btScalar angle = 10.0/radian;   // 回転角はラジアン単位で指定（OpenGLのglRotateはdegree）
    btQuaternion qrot(axis, angle); // 回転軸と回転角からクォータニオンを計算（OpenGLのglRotateとは引数の順番が逆）
    btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(qrot, pos));

    // 質量を0にすると衝突しても動かない静的剛体になる（慣性モーメントも必ず0になるので必要無い）
    groundBody = new btRigidBody(0.0, motion_state, collision_shape);
    // 反発係数の設定（衝突する物体の反発係数と衝突される物体の反発係数を掛けた値が最終的な反発係数になる）
    btScalar restitution = btScalar(0.8);
    groundBody->setRestitution(restitution);

    // ワールドに剛体オブジェクトを追加
    dynamicsWorld->addRigidBody(groundBody);
}

void deleter(){
    
}
