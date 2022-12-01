#ifndef _GUTENBERG_SCENE_H
#define _GUTENBERG_SCENE_H

#include "BulletWorld.h"
#include "model/MovingObject.h"
#include "model/StaticObject.h"
#include "shader/MultiLightShader.h"
#include "shader/SkyboxShader.h"
#include "learnopengl/camera.h"

class GutenbergScene {
protected:
    // shaders
    //Shader *multilightShader;
    MultiLightShader *multilightShader;
    SkyboxShader *skyboxShader;
    //Shader* pbrShader;
    Shader* equirectangularToCubemapShader;
    Shader* irradianceShader;
    Shader* prefilterShader;
    Shader* brdfShader;

    //Shader* backgroundShader;

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

    MovingObject *ball2;

    MovingObject* block1;
    MovingObject* block2;
    MovingObject* block3;
    MovingObject* block4;
    MovingObject* block5;
    MovingObject* block6;
    MovingObject* block7;
    MovingObject* block8;
    StaticObject* frame;

    //// gold
    //unsigned int goldAlbedoMap;
    //unsigned int goldNormalMap;
    //unsigned int goldMetallicMap;
    //unsigned int goldRoughnessMap;
    //unsigned int goldAOMap;

    unsigned int envCubemap;
    unsigned int irradianceMap;
    unsigned int prefilterMap;
    unsigned int brdfLUTTexture;

    // lights
// ------
    glm::vec3 lightPositions[4] = {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3(10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3(10.0f, -10.0f, 10.0f),
    };
    glm::vec3 lightColors[4] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };

public:
    GutenbergScene();
    ~GutenbergScene();

    void addPhysics(BulletWorld *world);
    void moveStatic(float time);
    void render(const glm::mat4& projection, const Camera& camera);

    Shader* pbrShader;
    Shader* backgroundShader;
};

#endif /* _GUTENBERG_SCENE_H */
