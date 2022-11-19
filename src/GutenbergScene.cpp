#include "GutenbergScene.h"
#include <iostream>

GutenbergScene::GutenbergScene()
{
    // shaders
    multilightShader = new MultiLightShader();

    // objects
	ground = new StaticObject("ground", multilightShader->getShader(), btVector3(0.1, 0.1, 0.1), btVector3(0, -0.1, 0), 0.5f, BOX_SHAPE_PROXYTYPE);

    btScalar hofst = 0.0;
    dom1 = new MovingObject("dom1", 10.0, multilightShader->getShader(), btVector3(0.5, 0.5, 0.5), btVector3(5, hofst + 0.2, 0), 0.5f, BOX_SHAPE_PROXYTYPE);
    dom2 = new MovingObject("dom2", 10.0, multilightShader->getShader(), btVector3(0.5, 0.5, 0.5), btVector3(4.5, hofst + 0.25, 0), 0.5f, BOX_SHAPE_PROXYTYPE);
    dom3 = new MovingObject("dom3", 10.0, multilightShader->getShader(), btVector3(0.5, 0.5, 0.5), btVector3(4, hofst + 0.3, 0), 0.5f, BOX_SHAPE_PROXYTYPE);
    dom4 = new MovingObject("dom4", 10.0, multilightShader->getShader(), btVector3(0.5, 0.5, 0.5), btVector3(3.5, hofst + 0.35, 0), 0.5f, BOX_SHAPE_PROXYTYPE);
    dom5 = new MovingObject("dom5", 10.0, multilightShader->getShader(), btVector3(0.5, 0.5, 0.5), btVector3(3, hofst + 0.4, 0), 0.5f, BOX_SHAPE_PROXYTYPE);
}

void GutenbergScene::addPhysics(BulletWorld *world)
{
    world->addRigidBody(ground->getBody());

    world->addRigidBody(dom1->getBody());
    world->addRigidBody(dom2->getBody());
    world->addRigidBody(dom3->getBody());
    world->addRigidBody(dom4->getBody());
    world->addRigidBody(dom5->getBody());
}

GutenbergScene::~GutenbergScene()
{
    delete multilightShader;

    delete ground;

    delete dom1;
    delete dom2;
    delete dom3;
    delete dom4;
    delete dom5;
}

void GutenbergScene::moveStatic(float time)
{
    const glm::vec3 AXIS_Y = glm::vec3(0.0f, 1.0f, 0.0f);

    btTransform trans;
    float buf[16];
    glm::mat4 model;

    // trans.setIdentity();
    // trans.setOrigin(concav_ground->getOriginalLocation());
    // trans.getOpenGLMatrix(buf);
    // model = glm::rotate(glm::make_mat4(buf), glm::radians(40 * time), AXIS_Y);
    // trans.setFromOpenGLMatrix(glm::value_ptr(model));
    // concav_ground->getBody()->getMotionState()->setWorldTransform(trans);
    // concav_ground->getBody()->setWorldTransform(trans);
}

void GutenbergScene::render(const glm::mat4& projection, const Camera& camera)
{
    multilightShader->use(camera.Position, camera.Front);
	multilightShader->getShader()->setMat4("projection", projection);
	multilightShader->getShader()->setMat4("view", camera.GetViewMatrix());
    ground->draw();
    dom1->draw();
    dom2->draw();
    dom3->draw();
    dom4->draw();
    dom5->draw();
    
}
