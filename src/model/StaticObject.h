#ifndef _STATIC_OBJECT_H
#define _STATIC_OBJECT_H

#include "btBulletDynamicsCommon.h"
#include "../learnopengl/model.h"

class StaticObject {
protected:
    btRigidBody* mBody;
    btTransform mWorldTrans;
    Model objModel;
    Shader objShader;
    btVector3 objScaling;

public:
    StaticObject(string objName, Shader* shader, btVector3 scaling, const btVector3& initLoc = btVector3(), btScalar friction = 0.5);
    virtual ~StaticObject();

    btRigidBody* getBody();
    virtual void draw();
};

#endif /* _STATIC_OBJECT_H */
