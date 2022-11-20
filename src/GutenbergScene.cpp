#include "GutenbergScene.h"
#include <iostream>

GutenbergScene::GutenbergScene()
{
    // shaders
    multilightShader = new MultiLightShader();

    // precious BALL
    btScalar ball_rad = 0.25f;
    // ball = new MovingObject("ball", 5.0, multilightShader->getShader(), btVector3(ball_rad / 2.54, ball_rad / 2.54, ball_rad / 2.54), btVector3(3.5, ball_rad + 2.0, -4.0), 0.5f, SPHERE_SHAPE_PROXYTYPE);
    ball = new MovingObject("ball", 8.0, multilightShader->getShader(), btVector3(ball_rad / 2.54, ball_rad / 2.54, ball_rad / 2.54), btVector3(3.2, ball_rad + 2.0, -4.0), 0.18f);

    // objects
	ground = new StaticObject("ground", multilightShader->getShader(), btVector3(0.1, 0.1, 0.1), btVector3(0, -0.1, 0), 0.5f, BOX_SHAPE_PROXYTYPE);
	rail = new StaticObject("rail", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(2.5, 0.05, 0.0), 0.3f);

    btScalar hofst = 0.0, zofst = -2.0, xofst = 0.5f;
    dom1 = new MovingObject("dom1", 10.0, multilightShader->getShader(), btVector3(0.5, 0.5, 0.5), btVector3(zofst + 0.4, hofst + 0.2, xofst), 0.5f, BOX_SHAPE_PROXYTYPE);
    dom2 = new MovingObject("dom2", 10.0, multilightShader->getShader(), btVector3(0.5, 0.5, 0.5), btVector3(zofst + 0.2, hofst + 0.25, xofst), 0.5f, BOX_SHAPE_PROXYTYPE);
    dom3 = new MovingObject("dom3", 10.0, multilightShader->getShader(), btVector3(0.5, 0.5, 0.5), btVector3(zofst + 0, hofst + 0.3, xofst), 0.5f, BOX_SHAPE_PROXYTYPE);
    dom4 = new MovingObject("dom4", 10.0, multilightShader->getShader(), btVector3(0.5, 0.5, 0.5), btVector3(zofst + -0.2, hofst + 0.35, xofst), 0.5f, BOX_SHAPE_PROXYTYPE);
    dom5 = new MovingObject("dom5", 10.0, multilightShader->getShader(), btVector3(0.5, 0.5, 0.5), btVector3(zofst + -0.4, hofst + 0.4, xofst), 0.5f, BOX_SHAPE_PROXYTYPE);
}

void GutenbergScene::addPhysics(BulletWorld *world)
{
    world->addRigidBody(ground->getBody());
    world->addRigidBody(rail->getBody());

    world->addRigidBody(ball->getBody());
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
    delete rail;

    delete ball;
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

    static bool exch = false;
    if (time > 7.0f && !exch) {
        exch = true;
        rail->getBody()->setFriction(0.0f);
        ball->getBody()->setCollisionShape(new btSphereShape(0.3));
        ball->getBody()->setFriction(0.0f);
    }

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
    rail->draw();
    ball->draw();
    dom1->draw();
    dom2->draw();
    dom3->draw();
    dom4->draw();
    dom5->draw();
    
}
