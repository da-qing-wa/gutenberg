#ifndef _MOVING_OBJECT_H
#define _MOVING_OBJECT_H

#include "btBulletDynamicsCommon.h"

class MovingObject {
protected:
    btRigidBody* mBody;
    btTransform mWorldTrans;

public:
    MovingObject(btScalar mass, btCollisionShape* shape, const btVector3 &initLoc = btVector3());
    virtual ~MovingObject();

    btRigidBody *getBody();
    virtual void draw();
};

#endif /* _MOVING_OBJECT_H */
