#include "BulletWorld.h"

BulletWorld::BulletWorld() {
    mCollisionConfiguration = new btDefaultCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
    mOverlappingPairCache = new btDbvtBroadphase();
    mSolver = new btSequentialImpulseConstraintSolver;
    mDynamicsWorld = new btDiscreteDynamicsWorld(
        mDispatcher, 
        mOverlappingPairCache, 
        mSolver, 
        mCollisionConfiguration
    );

    mDynamicsWorld->setGravity(btVector3(0, -9.8, 0));
}

BulletWorld::~BulletWorld() {
	for (int i = mDynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[i];
		mDynamicsWorld->removeCollisionObject(obj);
	}
    delete mDynamicsWorld;
    delete mSolver;
    delete mOverlappingPairCache;
    delete mDispatcher;
    delete mCollisionConfiguration;
}

btDiscreteDynamicsWorld* BulletWorld::getDynamicsWorld()
{
    return mDynamicsWorld;
}

void BulletWorld::addRigidBody(btRigidBody *body)
{
    mDynamicsWorld->addRigidBody(body);
}

void BulletWorld::step(float dt)
{
    mDynamicsWorld->stepSimulation(dt);
}
