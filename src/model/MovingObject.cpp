#include "MovingObject.h"
#include <iostream>

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
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
        shape = new btSphereShape((maxX - minX) * objScaling[0] / 2);

    }
    else if (shapeType == COMPOUND_SHAPE_PROXYTYPE)
    {
        //shape = new btCompoundShape();
    }
    else if (shapeType == BOX_SHAPE_PROXYTYPE)
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
        btConvexHullShape *convexshape = new btConvexHullShape();
        for (auto mh : objModel.meshes)
            for (auto mhv : mh.vertices)
                convexshape->addPoint(btVector3(mhv.Position.x, mhv.Position.y, mhv.Position.z), false);
        convexshape->recalcLocalAabb();
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
    delete mBody->getCollisionShape();
    delete mBody->getMotionState();
    delete mBody;
}

btRigidBody* MovingObject::getBody()
{
    return mBody;
}

void MovingObject::draw(Shader* shader)
{
    mBody->getMotionState()->getWorldTransform(mWorldTrans);
    btScalar buf[16];
    mWorldTrans.getOpenGLMatrix(buf);
    // render the loaded model
    glm::mat4 model = glm::make_mat4(buf);
    model = glm::scale(model, glm::vec3(objScaling[0], objScaling[1], objScaling[2]));
    if (shader == nullptr)
        shader = objShader;
    shader->use();
    shader->setMat4("model", model);
    objModel.Draw(shader);
}
