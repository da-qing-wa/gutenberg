#include "StaticObject.h"
#include <iostream>


static btBvhTriangleMeshShape* trimeshShape = 0;
static btRigidBody* staticBody = 0;
static float waveheight = 5.f;
const int NUM_VERTS_X = 30;
const int NUM_VERTS_Y = 30;
const int totalVerts = NUM_VERTS_X * NUM_VERTS_Y;
const float TRIANGLE_SIZE = 8.f;
int vertStride = sizeof(btVector3);
int indexStride = 3 * sizeof(int);
const int totalTriangles = 2 * (NUM_VERTS_X - 1) * (NUM_VERTS_Y - 1);

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

//void Meshes_to_GLInstanceVertexBuff(vector<Mesh>& meshes, btScalar* btScalarBuf)
//{
//    int p_buf = 0;
//    for (int i = 0; i < meshes.size(); i++)
//    {
//        for (int j = 0; j < meshes[i].vertices.size(); j++)
//        {
//            btScalarBuf[p_buf + 0] = meshes[i].vertices[j].Position[0];
//            btScalarBuf[p_buf + 1] = meshes[i].vertices[j].Position[1];
//            btScalarBuf[p_buf + 2] = meshes[i].vertices[j].Position[2];
//            btScalarBuf[p_buf + 3] = 1;
//            btScalarBuf[p_buf + 4] = meshes[i].vertices[j].Normal[0];
//            btScalarBuf[p_buf + 5] = meshes[i].vertices[j].Normal[1];
//            btScalarBuf[p_buf + 6] = meshes[i].vertices[j].Normal[2];
//            btScalarBuf[p_buf + 7] = meshes[i].vertices[j].TexCoords[0];
//            btScalarBuf[p_buf + 8] = meshes[i].vertices[j].TexCoords[1];
//            p_buf += 9;
//        }
//    }
//}

void setVertexPositions(float waveheight, float offset)
{
    int i;
    int j;

    for (i = 0; i < NUM_VERTS_X; i++)
    {
        for (j = 0; j < NUM_VERTS_Y; j++)
        {
            gVertices[i + j * NUM_VERTS_X].setValue((i - NUM_VERTS_X * 0.5f) * TRIANGLE_SIZE,
                //0.f,
                waveheight * sinf((float)i + offset) * cosf((float)j + offset),
                (j - NUM_VERTS_Y * 0.5f) * TRIANGLE_SIZE);
        }
    }
}

StaticObject::StaticObject(string objName, Shader& shader, btVector3 scaling, const btVector3& initLoc, btScalar friction)
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
    btScalar* btScalarBuf = new btScalar[vertices_num * 9 * sizeof(float)];

    //btVector3* gVertices = new btVector3[totalVerts];
    //int* gIndices = new int[totalTriangles * 3];

    //Meshes_to_GLInstanceVertexBuff(objModel.meshes, btScalarBuf);


    printf("[INFO] Obj loaded: Extracted %d verticed from obj file [%s]\n", vertices_num, objDir.c_str());

    // generate collision shape
    btCompoundShape* shape = new btCompoundShape();

    for (int i = 0; i < objModel.meshes.size(); i++)
    {
        btBvhTriangleMeshShape* tri_mesh_shape = 0;
        btVector3* gVertices = new btVector3[totalVerts];
        int* gIndices = new int[totalTriangles * 3];


        setVertexPositions(waveheight, 0.f);

        int index = 0;
        for (i = 0; i < NUM_VERTS_X - 1; i++)
        {
            for (int j = 0; j < NUM_VERTS_Y - 1; j++)
            {
                gIndices[index++] = j * NUM_VERTS_X + i;
                gIndices[index++] = j * NUM_VERTS_X + i + 1;
                gIndices[index++] = (j + 1) * NUM_VERTS_X + i + 1;

                gIndices[index++] = j * NUM_VERTS_X + i;
                gIndices[index++] = (j + 1) * NUM_VERTS_X + i + 1;
                gIndices[index++] = (j + 1) * NUM_VERTS_X + i;
            }
        }

        //btTriangleIndexVertexArray(int numTriangles, int* triangleIndexBase, int triangleIndexStride, int numVertices, btScalar* vertexBase, int vertexStride);
        btTriangleIndexVertexArray* m_indexVertexArrays = new btTriangleIndexVertexArray(totalTriangles,
            gIndices,
            indexStride,
            totalVerts, (btScalar*)&gVertices[0].x(), vertStride);


        bool useQuantizedAabbCompression = true;
        btVector3 aabbMin(-1000, -1000, -1000), aabbMax(1000, 1000, 1000);
        //shape = new btBvhTriangleMeshShape(m_indexVertexArrays, useQuantizedAabbCompression, aabbMin, aabbMax);

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

void StaticObject::draw()
{
    //mBody->getMotionState()->getWorldTransform(mWorldTrans);
    //btScalar buf[16];
    //mWorldTrans.getOpenGLMatrix(buf);
    //// render the loaded model
    //objShader.use();
    //glm::mat4 model = btScalar16_to_mat4(buf);
    //model = glm::scale(model, glm::vec3(objScaling[0], objScaling[1], objScaling[2]));
    //objShader.setMat4("model", model);
    //objModel.Draw(objShader);
}
