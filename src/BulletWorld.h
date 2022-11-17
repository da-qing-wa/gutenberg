#ifndef _BULLET_WORLD_H
#define _BULLET_WORLD_H

#include "btBulletDynamicsCommon.h"

class BulletWorld {
protected:
    btDefaultCollisionConfiguration* mCollisionConfiguration;
    btCollisionDispatcher* mDispatcher;
    btBroadphaseInterface* mOverlappingPairCache;
    btSequentialImpulseConstraintSolver* mSolver;
    btDiscreteDynamicsWorld* mDynamicsWorld;

public:
    BulletWorld();
    virtual ~BulletWorld();

    virtual void addRigidBody(btRigidBody *body);
    virtual void step(float dt);

};

#endif /* _BULLET_WORLD_H */
