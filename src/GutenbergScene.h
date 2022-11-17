#ifndef _GUTENBERG_SCENE_H
#define _GUTENBERG_SCENE_H

#include "BulletWorld.h"
#include "model/MovingObject.h"
#include "shader/MultiLightShader.h"
#include "learnopengl/camera.h"

class GutenbergScene {
protected:
    // shaders
    MultiLightShader *multilightShader;

    // objects
    MovingObject *ground;
    MovingObject *neck_walker;

public:
    GutenbergScene();
    ~GutenbergScene();

    void addPhysics(BulletWorld *world);
    void render(const glm::mat4& projection, const Camera& camera);
};

#endif /* _GUTENBERG_SCENE_H */
