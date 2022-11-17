#ifndef _STATIC_OBJECT_H
#define _STATIC_OBJECT_H

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "../learnopengl/model.h"

class MovingObject {
protected:
    btRigidBody* mBody;
    btTransform mWorldTrans;
    Model objModel;
    Shader objShader;
    btVector3 objScaling;

public:
    MovingObject(string objName, btScalar mass, Shader& shader, btVector3 scaling, const btVector3& initLoc = btVector3(), btScalar friction = 0.5, int shapeType = CONVEX_HULL_SHAPE_PROXYTYPE);
    virtual ~MovingObject();

    btRigidBody* getBody();
    virtual void draw();
};

#endif /* _STATIC_OBJECT_H */
