#include "MovingObject.h"
#include <iostream>

static glm::mat4 btScalar16_to_mat4(btScalar transform[16])
{
    glm::mat4 model = glm::mat4(
        transform[0], transform[1], transform[2], transform[3],
        transform[4], transform[5], transform[6], transform[7],
        transform[8], transform[9], transform[10], transform[11],
        transform[12], transform[13], transform[14], transform[15]
    );
    return model;
}

static void Meshes_to_GLInstanceVertexBuff(vector<Mesh>& meshes, btScalar* btScalarBuf)
{
    int p_buf = 0;
    for (int i = 0; i < meshes.size(); i++)
    {
        for (int j = 0; j < meshes[i].vertices.size(); j++)
        {
            btScalarBuf[p_buf + 0] = meshes[i].vertices[j].Position[0];
            btScalarBuf[p_buf + 1] = meshes[i].vertices[j].Position[1];
            btScalarBuf[p_buf + 2] = meshes[i].vertices[j].Position[2];
            btScalarBuf[p_buf + 3] = 1;
            btScalarBuf[p_buf + 4] = meshes[i].vertices[j].Normal[0];
            btScalarBuf[p_buf + 5] = meshes[i].vertices[j].Normal[1];
            btScalarBuf[p_buf + 6] = meshes[i].vertices[j].Normal[2];
            btScalarBuf[p_buf + 7] = meshes[i].vertices[j].TexCoords[0];
            btScalarBuf[p_buf + 8] = meshes[i].vertices[j].TexCoords[1];
            p_buf += 9;
        }
    }
}

MovingObject::MovingObject(string objName, btScalar mass, Shader *shader, btVector3 scaling, const btVector3& initLoc, btScalar friction, int shapeType)
{
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(initLoc);

    btVector3 localInertia(0, 0, 0);

    objShader = shader;
    objScaling = scaling;

    string objDir = "resources/objects/" + objName + "/" + objName + ".obj";
    objModel.setModel(objDir);

    int vertices_num = 0;
    for (int i = 0; i < objModel.meshes.size(); i++)
    {
        vertices_num += objModel.meshes[i].vertices.size();
    }

    printf("[INFO] Obj loaded: Extracted %d verticed from obj file [%s]\n", vertices_num, objDir.c_str());


    btCollisionShape* shape = 0;

    // generate collision shape
    if (shapeType == SPHERE_SHAPE_PROXYTYPE)
    {
        shape = new btSphereShape(scaling[0]);

    }
    else if (shapeType == COMPOUND_SHAPE_PROXYTYPE)
    {
        //shape = new btCompoundShape();
    }
    else
    {
        btScalar* btScalarBuf = new btScalar[vertices_num * 9 * sizeof(float)];
        Meshes_to_GLInstanceVertexBuff(objModel.meshes, btScalarBuf);
        btConvexHullShape *convexshape = new btConvexHullShape(btScalarBuf, vertices_num, 9 * sizeof(float));
        // set scaling
        convexshape->setLocalScaling(objScaling);
        convexshape->optimizeConvexHull();
        shape = convexshape;
    }
    if (mass > 1e-5)
        shape->calculateLocalInertia(mass, localInertia);
    btDefaultMotionState* mMotionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, mMotionState, shape, localInertia);
    rbInfo.m_friction = friction;
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
    // render the loaded model
    glm::mat4 model = btScalar16_to_mat4(buf);
    model = glm::scale(model, glm::vec3(objScaling[0], objScaling[1], objScaling[2]));
    objShader->setMat4("model", model);
    objModel.Draw(objShader);
}
