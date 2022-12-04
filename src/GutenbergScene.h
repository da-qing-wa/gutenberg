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
    Shader* equirectangularToCubemapShader;
    Shader* irradianceShader;
    Shader* prefilterShader;
    Shader* brdfShader;

    // objects
    StaticObject *ground;
    StaticObject *rounddesk;

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
    MovingObject* block9;
    StaticObject* desk_lamp;
    StaticObject* clock;
    StaticObject* lime;
    StaticObject* potted_plant;

    unsigned int envCubemap;
    unsigned int irradianceMap;
    unsigned int prefilterMap;
    unsigned int brdfLUTTexture;

    // lights
    // ------
    glm::vec3 lightPositions[4] = {
        glm::vec3(-150.156f,  78.2362f, -137.485f),
        glm::vec3(-130.156f,  78.2362f, -137.485f),
        glm::vec3(-150.156f,  78.2362f, -117.485f),
        glm::vec3(-155.156f,  78.2362f, -147.485f),
    };
    glm::vec3 lightColors[4] = {
        glm::vec3(30000.0f, 30000.0f, 30000.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
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
