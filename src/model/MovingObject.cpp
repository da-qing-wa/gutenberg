#include "MovingObject.h"

#include <iostream>

MovingObject::MovingObject(
    btScalar mass, 
    btCollisionShape* shape, 
    const btVector3 &initLoc)
{
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(initLoc);
    
    btVector3 localInertia(0, 0, 0);
    if (mass > 1e-5)
        shape->calculateLocalInertia(mass, localInertia);
    
    btDefaultMotionState *mMotionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, mMotionState, shape, localInertia);
    mBody = new btRigidBody(rbInfo);
}

MovingObject::~MovingObject()
{
    delete mBody->getMotionState();
    delete mBody;
}

btRigidBody* MovingObject::getBody()
{
    return mBody;
}

void MovingObject::draw()
{
    mBody->getMotionState()->getWorldTransform(mWorldTrans);
    btScalar buf[16];
    mWorldTrans.getOpenGLMatrix(buf);
    std::cout << buf[13] << std::endl;
}
