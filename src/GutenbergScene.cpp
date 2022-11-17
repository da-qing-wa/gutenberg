#include "GutenbergScene.h"

GutenbergScene::GutenbergScene()
{
    // shaders
    multilightShader = new MultiLightShader();

    // objects
	ground = new MovingObject("ground", 0, multilightShader->getShader(), btVector3(0.01, 0.01, 0.01), btVector3(0, 0, 0));
	neck_walker = new MovingObject("neck_walker", 1.0, multilightShader->getShader(), btVector3(0.1, 0.1, 0.1), btVector3(0, 10, 0));
}

void GutenbergScene::addPhysics(BulletWorld *world)
{
    world->addRigidBody(ground->getBody());
    world->addRigidBody(neck_walker->getBody());
}

GutenbergScene::~GutenbergScene()
{
    delete multilightShader;

    delete ground;
    delete neck_walker;
}

void GutenbergScene::render(const glm::mat4& projection, const Camera& camera)
{
    multilightShader->use(camera.Position, camera.Front);
	multilightShader->getShader()->setMat4("projection", projection);
	multilightShader->getShader()->setMat4("view", camera.GetViewMatrix());
    //ground->draw();
    ground->draw();
    neck_walker->draw();
    
}
