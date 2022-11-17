#ifndef _BULLET_WORLD_H
#define _BULLET_WORLD_H

#include "btBulletDynamicsCommon.h"

class BulletWorld {
protected:
    btDefaultCollisionConfiguration* mCollisionConfiguration;
    btCollisionDispatcher* mDispatcher;
    btBroadphaseInterface* mOverlappingPairCache;
    btSequentialImpulseConstraintSolver* mSolver;
    
    btClock mClock;
public:
    BulletWorld();
    virtual ~BulletWorld();

    virtual void addRigidBody(btRigidBody *body);
    virtual void step(float dt);
    btClock& getClock();
    btDiscreteDynamicsWorld* mDynamicsWorld;
};


#endif /* _BULLET_WORLD_H */
