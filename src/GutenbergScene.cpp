#include "GutenbergScene.h"
#include <iostream>

GutenbergScene::GutenbergScene()
{
    // shaders
    multilightShader = new MultiLightShader();
    skyboxShader = new SkyboxShader();

    // precious BALL
    btScalar ball_rad = 8.5f;
    // ball = new MovingObject("ball", 5.0, multilightShader->getShader(), btVector3(ball_rad / 2.54, ball_rad / 2.54, ball_rad / 2.54), btVector3(3.5, ball_rad + 2.0, -4.0), 0.5f, SPHERE_SHAPE_PROXYTYPE);
    ball = new MovingObject("ball", 8.0, multilightShader->getShader(), btVector3(ball_rad / 2.54, ball_rad / 2.54, ball_rad / 2.54), btVector3(-15, 137.0, -135.0), 0.18f);
    // ball = new MovingObject("ball", 8.0, multilightShader->getShader(), btVector3(ball_rad / 2.54, ball_rad / 2.54, ball_rad / 2.54), btVector3(0.0, 0.0, 0.0), 0.18f);

    // objects
	ground = new StaticObject("ground", multilightShader->getShader(), btVector3(5.0, 0.1, 5.0), btVector3(0, -23.1, 0), 0.5f, BOX_SHAPE_PROXYTYPE);

	rail = new StaticObject("rail", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(8,7,0), 0.3f);
    wm_blade = new StaticObject("wm_blade", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(95.74, 46.81, -136.0), 0.3f);
    //wm_blade = new StaticObject("wm_blade", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(106.74, 134.81, 37.555), 0.3f);
    // wm_blade = new StaticObject("wm_blade", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-106.69, 48.974, 11.166), 0.3f);
    wm_body = new StaticObject("wm_body", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-5,0,-50), 0.3f);


    try3 = new StaticObject("try3", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0,0,0), 0.03f);
    try6 = new StaticObject("try6", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(0, 0, 0), 0.03f);
    try7 = new StaticObject("try7", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-28, 0, 0), 0.3f);

     dom1 = new MovingObject("dom1", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-108.33, -10.395, -0.28), 0.5f, BOX_SHAPE_PROXYTYPE);
     dom2 = new MovingObject("dom2", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-123.87, -7.2869, -0.28), 0.5f, BOX_SHAPE_PROXYTYPE);
     dom3 = new MovingObject("dom3", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-139.41, -4.7189, -0.28), 0.5f, BOX_SHAPE_PROXYTYPE);
     dom4 = new MovingObject("dom4", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-154.95, -1.0708, -0.28), 0.5f, BOX_SHAPE_PROXYTYPE);
     dom5 = new MovingObject("dom5", 10.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-170.49, 2.3072, -0.28), 0.5f, BOX_SHAPE_PROXYTYPE);

     ball2 = new MovingObject("ball", 50.0, multilightShader->getShader(), btVector3(ball_rad / 2.54, ball_rad / 2.54, ball_rad / 2.54), btVector3(-190.0, 36.28, -2.0), 0.001f);

     float block_friction = 0.5f;
     block1 = new MovingObject("block1", 3.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-355.27, 6.4494, 21.743), block_friction);
     block2 = new MovingObject("block2", 3.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-353.23, 8.0415, 26.372), block_friction);
     block3 = new MovingObject("block3", 3.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-355.6, -2.9584, 20.977), block_friction);
     block4 = new MovingObject("block4", 3.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-351.1, 6.4494, 31.185), block_friction);
     block5 = new MovingObject("block5", 3.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-350.85, -2.9584, 31.767), block_friction);
     block6 = new MovingObject("block6", 3.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-355.77, -12.8, 21.743), block_friction);
     block7 = new MovingObject("block7", 3.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-353.23, -13.958, 26.372), block_friction);
     block8 = new MovingObject("block8", 3.0, multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-351.1, -12.8, 31.185), block_friction);
     //frame = new StaticObject("frame", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-353.057, -3.8399, 27.755), 0.5f);
     frame = new StaticObject("frame", multilightShader->getShader(), btVector3(1.0, 1.0, 1.0), btVector3(-353.057, -3.8399, 27.755), 0.5f);



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
     world->addRigidBody(dom1->getBody());
     world->addRigidBody(dom2->getBody());
     world->addRigidBody(dom3->getBody());
     world->addRigidBody(dom4->getBody());
     world->addRigidBody(dom5->getBody());

     world->addRigidBody(ball2->getBody());

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
    delete skyboxShader;

    delete ground;
    delete rail;
    delete wm_blade;
    delete wm_body;
    delete try3;
    delete try6;
    delete try7;

    delete ball;
    delete dom1;
    delete dom2;
    delete dom3;
    delete dom4;
    delete dom5;

    delete ball2;

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
    if (time > 35.0f && !exch) {
        exch = true;
         rail->getBody()->setFriction(0.0f);
         ball->getBody()->setCollisionShape(new btSphereShape(8.5f));
         ball->getBody()->setFriction(0.0f);
         ball2->getBody()->setCollisionShape(new btSphereShape(8.5f));
         ball2->getBody()->setFriction(0.0f);
    }

    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(wm_blade->getOriginalLocation().getX(), wm_blade->getOriginalLocation().getY(), wm_blade->getOriginalLocation().getZ()));
    model = glm::rotate(model, glm::radians(140.0f), AXIS_Z);
    //model = glm::rotate(model, glm::radians(40 * time+20), AXIS_Z);
    
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
    dom1->draw();
    dom2->draw();
    dom3->draw();
    dom4->draw();
    dom5->draw();

    ball2->draw();

    block1->draw();
    block2->draw();
    block3->draw();
    block4->draw();
    block5->draw();
    block6->draw();
    block7->draw();
    block8->draw();
    frame->draw();

    skyboxShader->use(projection,camera);
}
