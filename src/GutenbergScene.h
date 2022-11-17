#ifndef _GUTENBERG_SCENE_H
#define _GUTENBERG_SCENE_H

#include "BulletWorld.h"
#include "model/MovingObject.h"
#include "model/StaticObject.h"
#include "shader/MultiLightShader.h"
#include "learnopengl/camera.h"

class GutenbergScene {
protected:
    // shaders
    MultiLightShader *multilightShader;

    // objects
    //MovingObject *ground;
    StaticObject *concav_ground;
    MovingObject *neck_walker;

public:
    GutenbergScene();
    ~GutenbergScene();

    void addPhysics(BulletWorld *world);
    void setWorldTransform(BulletWorld* world,float time);
    void render(const glm::mat4& projection, const Camera& camera,float time);
};

#endif /* _GUTENBERG_SCENE_H */
