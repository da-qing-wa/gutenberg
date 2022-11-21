#include "GutenbergScene.h"
#include <iostream>

GutenbergScene::GutenbergScene()
{
    // shaders
    multilightShader = new MultiLightShader();

    // precious BALL
    btScalar ball_rad = 9.0f;
    // ball = new MovingObject("ball", 5.0, multilightShader->getShader(), btVector3(ball_rad / 2.54, ball_rad / 2.54, ball_rad / 2.54), btVector3(3.5, ball_rad + 2.0, -4.0), 0.5f, SPHERE_SHAPE_PROXYTYPE);
    ball = new MovingObject("ball", 8.0, multilightShader->getShader(), btVector3(ball_rad / 2.54, ball_rad / 2.54, ball_rad / 2.54), btVector3(33, 184.0, -130.0), 0.1f);
    // ball = new MovingObject("ball", 8.0, multilightShader->getShader(), btVector3(ball_rad / 2.54, ball_rad / 2.54, ball_rad / 2.54), btVector3(0.0, 0.0, 0.0), 0.18f);

    // objects
	ground = new StaticObject("ground", multilightShader->getShader(), btVector3(10.0, 0.1, 10.0), btVector3(0, -23.1, 0), 0.5f, BOX_SHAPE_PROXYTYPE);

	rail = new StaticObject("rail", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0,0,0), 0.3f);
    wm_blade = new StaticObject("wm_blade", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(106.74, 134.81, 37.555), 0.3f);
    //wm_blade = new StaticObject("wm_blade", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(106.74, 134.81, 37.555), 0.3f);
    // wm_blade = new StaticObject("wm_blade", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-106.69, 48.974, 11.166), 0.3f);
    wm_body = new StaticObject("wm_body", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0,0,0), 0.3f);

    //try3 = new StaticObject("try3", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), 0.3f);
    //try6 = new StaticObject("try6", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), 0.3f);
    //try7 = new StaticObject("try7", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0.0, 0.0, 0.0), 0.3f);

    try3 = new StaticObject("try3", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0,0,0), 0.3f);
    try6 = new StaticObject("try6", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0, 0, 0), 0.3f);
    //try7 = new StaticObject("try7", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-50, 0, 0), 0.3f)

    //btVector3 dom_coordinate = btVector3(15.994, 9.0411, -22.827);
     dom1 = new MovingObject("dom1", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-108.33, -0.28, -10.395), 0.5f, BOX_SHAPE_PROXYTYPE);
     dom2 = new MovingObject("dom2", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-123.87, -0.28, -7.2869), 0.5f, BOX_SHAPE_PROXYTYPE);
     dom3 = new MovingObject("dom3", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-139.41, -0.28, -4.7189), 0.5f, BOX_SHAPE_PROXYTYPE);
     dom4 = new MovingObject("dom4", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-154.95, -0.28, -1.0708), 0.5f, BOX_SHAPE_PROXYTYPE);
     dom5 = new MovingObject("dom5", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-170.49, -0.28,  2.3072), 0.5f, BOX_SHAPE_PROXYTYPE);

     block1 = new MovingObject("block1", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-355.27, -21.743, 6.4494), 0.5f);
     block2 = new MovingObject("block2", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-353.23, -26.372, -8.0415), 0.5f);
     block3 = new MovingObject("block3", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-355.6, -20.977, -2.9584), 0.5f);
     block4 = new MovingObject("block4", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-351.1, -31.185, -6.4494), 0.5f);
     block5 = new MovingObject("block5", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-350.85, -31.767, -2.9584), 0.5f);
     block6 = new MovingObject("block6", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-355.77, -21.743, -12.8), 0.5f);
     block7 = new MovingObject("block7", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-353.23, -26.372, -13.958), 0.5f);
     block8 = new MovingObject("block8", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-351.1, -31.185, -12.8), 0.5f);
     frame = new StaticObject("frame", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-353.057, -27.755, -3.8399), 0.5f);



}

void GutenbergScene::addPhysics(BulletWorld *world)
{
    world->addRigidBody(ground->getBody());
    world->addRigidBody(rail->getBody());
    world->addRigidBody(wm_blade->getBody());
    world->addRigidBody(wm_body->getBody());
    world->addRigidBody(try3->getBody());
    world->addRigidBody(try6->getBody());
    //world->addRigidBody(try7->getBody());

    world->addRigidBody(ball->getBody());
     world->addRigidBody(dom1->getBody());
     world->addRigidBody(dom2->getBody());
     world->addRigidBody(dom3->getBody());
     world->addRigidBody(dom4->getBody());
     world->addRigidBody(dom5->getBody());

     world->addRigidBody(block1->getBody());
     world->addRigidBody(block2->getBody());
     world->addRigidBody(block3->getBody());
     world->addRigidBody(block4->getBody());
     world->addRigidBody(block5->getBody());
     world->addRigidBody(block6->getBody());
     world->addRigidBody(block7->getBody());
     world->addRigidBody(block8->getBody());
     world->addRigidBody(frame->getBody());
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
    //delete try7;

    delete ball;
     delete dom1;
     delete dom2;
     delete dom3;
     delete dom4;
     delete dom5;

     delete block1;
     delete block2;
     delete block3;
     delete block4;
     delete block5;
     delete block6;
     delete block7;
     delete block8;
     delete frame;
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

    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(wm_blade->getOriginalLocation().getX(), wm_blade->getOriginalLocation().getY(), wm_blade->getOriginalLocation().getZ()));
    model = glm::rotate(model, glm::radians(40 * time), AXIS_Z);
    
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
    //try7->draw();

    ball->draw();
    dom1->draw();
     dom2->draw();
     dom3->draw();
     dom4->draw();
     dom5->draw();

     block1->draw();
     block2->draw();
     block3->draw();
     block4->draw();
     block5->draw();
     block6->draw();
     block7->draw();
     block8->draw();
     frame->draw();
    
}
