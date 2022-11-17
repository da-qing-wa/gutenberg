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

void GutenbergScene::setWorldTransform(BulletWorld* world,float time)
{
    btScalar buf[16];
    concav_ground->getWorldTrans()->getOpenGLMatrix(buf);
    glm::mat4 model = glm::make_mat4(buf);
    model = glm::rotate(model, glm::radians(12 * time), glm::vec3(0, 1, 0));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            buf[i * 4 + j] = model[i][j];
            std::cout << buf[i * 4 + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    concav_ground->getWorldTrans()->setFromOpenGLMatrix(buf);

    
    //concav_ground->getBody()->getMotionState()->setWorldTransform(*(concav_ground->getWorldTrans()));
    concav_ground->getBody()->setWorldTransform(*(concav_ground->getWorldTrans()));
}

void GutenbergScene::render(const glm::mat4& projection, const Camera& camera,float time)
{
    multilightShader->use(camera.Position, camera.Front);
	multilightShader->getShader()->setMat4("projection", projection);
	multilightShader->getShader()->setMat4("view", camera.GetViewMatrix());
    //ground->draw();
    concav_ground->draw(time);
    neck_walker->draw();
    
}
