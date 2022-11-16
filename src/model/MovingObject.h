#ifndef _MOVING_OBJECT_H
#define _MOVING_OBJECT_H

#include "btBulletDynamicsCommon.h"
#include "../learnopengl/model.h"

class MovingObject {
protected:
    btRigidBody* mBody;
    btTransform mWorldTrans;
    Model objModel;
    Shader objShader;
    btVector3 objScaling;

public:
    MovingObject(string objName, btScalar mass, Shader& shader, btVector3 scaling, const btVector3 &initLoc = btVector3());
    virtual ~MovingObject();

    btRigidBody *getBody();
    virtual void draw();
};

#endif /* _MOVING_OBJECT_H */
