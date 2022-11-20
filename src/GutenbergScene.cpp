#include "GutenbergScene.h"
#include <iostream>

GutenbergScene::GutenbergScene()
{
    // shaders
    multilightShader = new MultiLightShader();

    // precious BALL
    btScalar ball_rad = 7.0f;
    // ball = new MovingObject("ball", 5.0, multilightShader->getShader(), btVector3(ball_rad / 2.54, ball_rad / 2.54, ball_rad / 2.54), btVector3(3.5, ball_rad + 2.0, -4.0), 0.5f, SPHERE_SHAPE_PROXYTYPE);
    ball = new MovingObject("ball", 8.0, multilightShader->getShader(), btVector3(ball_rad / 2.54, ball_rad / 2.54, ball_rad / 2.54), btVector3(0.0, 134.0, -134.0), 0.18f);
    // ball = new MovingObject("ball", 8.0, multilightShader->getShader(), btVector3(ball_rad / 2.54, ball_rad / 2.54, ball_rad / 2.54), btVector3(0.0, 0.0, 0.0), 0.18f);

    // objects
	ground = new StaticObject("ground", multilightShader->getShader(), btVector3(10.0, 0.1, 10.0), btVector3(0, -23.1, 0), 0.5f, BOX_SHAPE_PROXYTYPE);

	rail = new StaticObject("rail", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), 0.3f);
    wm_blade = new StaticObject("wm_blade", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), 0.3f);
    // wm_blade = new StaticObject("wm_blade", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-106.69, 48.974, 11.166), 0.3f);
    wm_body = new StaticObject("wm_body", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), 0.3f);

    try3 = new StaticObject("try3", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), 0.3f);
    try6 = new StaticObject("try6", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), 0.3f);
    try7 = new StaticObject("try7", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), 0.3f);

    // dom1 = new MovingObject("dom1", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(15.994, 9.0411, -22.827), 0.5f, BOX_SHAPE_PROXYTYPE);
    // dom2 = new MovingObject("dom2", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(15.994, 9.0411, -22.827), 0.5f, BOX_SHAPE_PROXYTYPE);
    // dom3 = new MovingObject("dom3", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(15.994, 9.0411, -22.827), 0.5f, BOX_SHAPE_PROXYTYPE);
    // dom4 = new MovingObject("dom4", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(15.994, 9.0411, -22.827), 0.5f, BOX_SHAPE_PROXYTYPE);
    // dom5 = new MovingObject("dom5", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(15.994, 9.0411, -22.827), 0.5f, BOX_SHAPE_PROXYTYPE);
}

void GutenbergScene::addPhysics(BulletWorld *world)
{
    world->addRigidBody(ground->getBody());
    world->addRigidBody(rail->getBody());
    world->addRigidBody(wm_blade->getBody());
    world->addRigidBody(wm_body->getBody());
    world->addRigidBody(try3->getBody());
    world->addRigidBody(try6->getBody());
    world->addRigidBody(try7->getBody());

    world->addRigidBody(ball->getBody());
    // world->addRigidBody(dom1->getBody());
    // world->addRigidBody(dom2->getBody());
    // world->addRigidBody(dom3->getBody());
    // world->addRigidBody(dom4->getBody());
    // world->addRigidBody(dom5->getBody());
}

GutenbergScene::~GutenbergScene()
{
    delete multilightShader;

    delete ground;
    delete rail;
    delete wm_blade;
    delete wm_body;
    delete try3;
    delete try6;
    delete try7;

    delete ball;
    // delete dom1;
    // delete dom2;
    // delete dom3;
    // delete dom4;
    // delete dom5;
}

void GutenbergScene::moveStatic(float time)
{
    const glm::vec3 AXIS_Z = glm::vec3(0.0f, 0.0f, 1.0f);

    btTransform trans;
    float buf[16];
    glm::mat4 model;

    static bool exch = false;
    if (time > 7.0f && !exch) {
        exch = true;
        // rail->getBody()->setFriction(0.0f);
        // ball->getBody()->setCollisionShape(new btSphereShape(0.3));
        // ball->getBody()->setFriction(0.0f);
    }

    btVector3 wmd(106.69, -48.974, -11.166);
    model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(wmd.x(), wmd.y(), wmd.z()));
    // model = glm::rotate(model, glm::radians(40 * time), AXIS_Z);
    // model = glm::translate(model, glm::vec3(-wmd.x(), -wmd.y(), -wmd.z()));
    trans.setFromOpenGLMatrix(glm::value_ptr(model));
    wm_blade->getBody()->getMotionState()->setWorldTransform(trans);
    wm_blade->getBody()->setWorldTransform(trans);
}

void GutenbergScene::render(const glm::mat4& projection, const Camera& camera)
{
    multilightShader->use(camera.Position, camera.Front);
	multilightShader->getShader()->setMat4("projection", projection);
	multilightShader->getShader()->setMat4("view", camera.GetViewMatrix());
    ground->draw();
    rail->draw();
    wm_blade->draw();
    wm_body->draw();
    try3->draw();
    try6->draw();
    try7->draw();

    ball->draw();
    // dom1->draw();
    // dom2->draw();
    // dom3->draw();
    // dom4->draw();
    // dom5->draw();
    
}
