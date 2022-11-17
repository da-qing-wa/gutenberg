#include "StaticObject.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


StaticObject::StaticObject(string objName, Shader* shader, btVector3 scaling, const btVector3& initLoc, btScalar friction)
{
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(initLoc);

    btVector3 localInertia(0, 0, 0);

    objShader = *shader;
    objScaling = scaling;

    string objDir = "resources/objects/" + objName + "/" + objName + ".obj";
    objModel.setModel(objDir);

    int vertices_num = 0;
    for (int i = 0; i < objModel.meshes.size(); i++)
    {
        vertices_num += objModel.meshes[i].vertices.size();
    }
    btScalar* btScalarBuf = new btScalar[vertices_num * 9 * sizeof(float)];

    printf("[INFO] Obj loaded: Extracted %d verticed from obj file [%s]\n", vertices_num, objDir.c_str());

    // generate collision shape
    btCompoundShape* shape = new btCompoundShape();

    for (int i = 0; i < objModel.meshes.size(); i++)
    {
        btVector3* gVertices = new btVector3[objModel.meshes[i].vertices.size()];
        int* gIndices = new int[objModel.meshes[i].indices.size()];

        for (int j = 0; j < objModel.meshes[i].vertices.size(); j++)
        {
            gVertices[j].setX(objModel.meshes[i].vertices[j].Position.x);
            gVertices[j].setY(objModel.meshes[i].vertices[j].Position.y);
            gVertices[j].setZ(objModel.meshes[i].vertices[j].Position.z);
        }
        for (int j = 0; j < objModel.meshes[i].indices.size(); j++)
        {
            gIndices[j] = objModel.meshes[i].indices[j];
        }

        btTriangleIndexVertexArray* m_indexVertexArrays = new btTriangleIndexVertexArray(
            objModel.meshes[i].indices.size() / 3,
            gIndices,
            3 * sizeof(int),
            objModel.meshes[i].vertices.size(),
            (btScalar*)&gVertices[0].x(),
            sizeof(btVector3)
        );

        btBvhTriangleMeshShape* tri_mesh_shape = new btBvhTriangleMeshShape(m_indexVertexArrays, true);
        btTransform* trans = new btTransform;
        trans->setIdentity();
        shape->addChildShape(*trans, tri_mesh_shape);
    }

    shape->setLocalScaling(objScaling);
    btDefaultMotionState* mMotionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(0, mMotionState, shape, localInertia);
    rbInfo.m_friction = friction;
    mBody = new btRigidBody(rbInfo);

}

StaticObject::~StaticObject()
{
    delete mBody->getMotionState();
    delete mBody;
}

btRigidBody* StaticObject::getBody()
{
    return mBody;
}

btTransform* StaticObject::getWorldTrans()
{
    return &mWorldTrans;
}

void StaticObject::draw(float time)
{
    mBody->getMotionState()->getWorldTransform(mWorldTrans);
    //mWorldTrans = mBody->getWorldTransform();
    btScalar buf[16];
    mWorldTrans.getOpenGLMatrix(buf);
    // render the loaded model
    objShader.use();
    glm::mat4 model = glm::make_mat4(buf);
    model = glm::rotate(model, glm::radians(12 * time), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(objScaling[0], objScaling[1], objScaling[2]));
    objShader.setMat4("model", model);
    objModel.Draw(&objShader);
}
