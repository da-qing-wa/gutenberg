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
    btVector3 origLoc;

public:
    StaticObject(
        string objName, 
        Shader* shader, 
        btVector3 scaling, 
        const btVector3& initLoc = btVector3(), 
        btScalar friction = 0.5, 
        int shapeType = TRIANGLE_MESH_SHAPE_PROXYTYPE
    );
    virtual ~StaticObject();

    btRigidBody* getBody();
    const btVector3 getOriginalLocation() const;
    virtual void draw(Shader* shader = nullptr);
};

#endif /* _STATIC_OBJECT_H */
