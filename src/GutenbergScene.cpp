#include "GutenbergScene.h"
#include<iostream>

GutenbergScene::GutenbergScene()
{
    // shaders
    multilightShader = new MultiLightShader();

    // objects
	//ground = new MovingObject("ground", 0, multilightShader->getShader(), btVector3(0.01, 0.01, 0.01), btVector3(0, 0, 0));
    concav_ground = new StaticObject("concav_ground", multilightShader->getShader(), btVector3(0.3, 0.3, 0.3), btVector3(0, 0, 0));
	neck_walker = new MovingObject("neck_walker", 1.0, multilightShader->getShader(), btVector3(0.1, 0.1, 0.1), btVector3(0, 10, 0));
}

void GutenbergScene::addPhysics(BulletWorld *world)
{
    //world->addRigidBody(ground->getBody());
    world->addRigidBody(concav_ground->getBody());
    world->addRigidBody(neck_walker->getBody());
}

GutenbergScene::~GutenbergScene()
{
    delete multilightShader;

    //delete ground;
    delete concav_ground;
    delete neck_walker;
}

void GutenbergScene::moveStatic(float time)
{
    const glm::vec3 AXIS_Y = glm::vec3(0.0f, 1.0f, 0.0f);

    btTransform trans;
    float buf[16];
    glm::mat4 model;

    trans.setIdentity();
    trans.setOrigin(concav_ground->getOriginalLocation());
    trans.getOpenGLMatrix(buf);
    model = glm::rotate(glm::make_mat4(buf), glm::radians(12 * time), AXIS_Y);
    trans.setFromOpenGLMatrix(glm::value_ptr(model));
    concav_ground->getBody()->getMotionState()->setWorldTransform(trans);
    concav_ground->getBody()->setWorldTransform(trans);
}

void GutenbergScene::render(const glm::mat4& projection, const Camera& camera)
{
    multilightShader->use(camera.Position, camera.Front);
	multilightShader->getShader()->setMat4("projection", projection);
	multilightShader->getShader()->setMat4("view", camera.GetViewMatrix());
    //ground->draw();
    concav_ground->draw();
    neck_walker->draw();
    
}
