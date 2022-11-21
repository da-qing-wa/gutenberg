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
    StaticObject *ground;
    StaticObject *rail;

    StaticObject *wm_blade;
    StaticObject *wm_body;

    StaticObject *try3;
    StaticObject *try6;
    StaticObject *try7;

    MovingObject *ball;
    
    MovingObject *dom1;
    MovingObject *dom2;
    MovingObject *dom3;
    MovingObject *dom4;
    MovingObject *dom5;

    MovingObject* block1;
    MovingObject* block2;
    MovingObject* block3;
    MovingObject* block4;
    MovingObject* block5;
    MovingObject* block6;
    MovingObject* block7;
    MovingObject* block8;
    StaticObject* frame;

public:
    GutenbergScene();
    ~GutenbergScene();

    void addPhysics(BulletWorld *world);
    void moveStatic(float time);
    void render(const glm::mat4& projection, const Camera& camera);
};

#endif /* _GUTENBERG_SCENE_H */
