#include "StaticObject.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

StaticObject::StaticObject(string objName, Shader* shader, btVector3 scaling, const btVector3& initLoc, btScalar friction, int shapeType)
{
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(initLoc);

    btVector3 localInertia(0, 0, 0);

    objShader = *shader;
    objScaling = scaling;
    origLoc = initLoc;

    string objDir = "resources/objects/" + objName + "/" + objName + ".obj";
    objModel.setModel(objDir);

    int vertices_num = 0;
    for (int i = 0; i < objModel.meshes.size(); i++)
    {
        vertices_num += objModel.meshes[i].vertices.size();
    }

    printf("[INFO] Obj loaded: Extracted %d verticed from obj file [%s]\n", vertices_num, objDir.c_str());

    // generate collision shape
    btCollisionShape* shape = 0;

    if (shapeType == BOX_SHAPE_PROXYTYPE)
    {
        btScalar minX = 1e9, minY = 1e9, minZ = 1e9;
        btScalar maxX = -1e9, maxY = -1e9, maxZ = -1e9;
        for (const Vertex &vt : objModel.meshes[0].vertices) {
            minX = glm::min(minX, vt.Position.x);
            minY = glm::min(minY, vt.Position.y);
            minZ = glm::min(minZ, vt.Position.z);
            maxX = glm::max(maxX, vt.Position.x);
            maxY = glm::max(maxY, vt.Position.y);
            maxZ = glm::max(maxZ, vt.Position.z);
        }
        btBoxShape *boxshape = new btBoxShape(btVector3(
            (maxX-minX) * objScaling[0] / 2, 
            (maxY-minY) * objScaling[1] / 2, 
            (maxZ-minZ) * objScaling[2] / 2
        ));
        // boxshape->setLocalScaling(objScaling);
        shape = boxshape;
    } 
    else 
    {
        btCompoundShape* compshape = new btCompoundShape();

        btTransform trans;
        trans.setIdentity();
        for (int i = 0; i < objModel.meshes.size(); i++)
        {

            btTriangleIndexVertexArray* m_indexVertexArrays = new btTriangleIndexVertexArray(
                objModel.meshes[i].indices.size() / 3,
                (int*)objModel.meshes[i].indices.data(),
                3 * sizeof(int),
                objModel.meshes[i].vertices.size(),
                (btScalar*)(glm::value_ptr(objModel.meshes[i].vertices[0].Position)),
                sizeof(Vertex)
            );

            btBvhTriangleMeshShape* tri_mesh_shape = new btBvhTriangleMeshShape(m_indexVertexArrays, true);
            tri_mesh_shape->buildOptimizedBvh();
            tri_mesh_shape->recalcLocalAabb();
            compshape->addChildShape(trans, tri_mesh_shape);
        }
        compshape->setLocalScaling(objScaling);
        shape = compshape;
    }
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

const btVector3 StaticObject::getOriginalLocation() const
{
    return origLoc;
}

void StaticObject::draw()
{
    mBody->getMotionState()->getWorldTransform(mWorldTrans);
    btScalar buf[16];
    mWorldTrans.getOpenGLMatrix(buf);
    // render the loaded model
    objShader.use();
    glm::mat4 model = glm::make_mat4(buf);
    model = glm::scale(model, glm::vec3(objScaling[0], objScaling[1], objScaling[2]));
    objShader.setMat4("model", model);
    objModel.Draw(&objShader);
}
