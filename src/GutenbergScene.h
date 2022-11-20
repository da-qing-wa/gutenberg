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

    MovingObject *ball;
    
    MovingObject *dom1;
    MovingObject *dom2;
    MovingObject *dom3;
    MovingObject *dom4;
    MovingObject *dom5;

public:
    GutenbergScene();
    ~GutenbergScene();

    void addPhysics(BulletWorld *world);
    void moveStatic(float time);
    void render(const glm::mat4& projection, const Camera& camera);
};

#endif /* _GUTENBERG_SCENE_H */
