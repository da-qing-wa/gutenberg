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

    StaticObject *table;
    StaticObject *slide1;
    StaticObject *slide2;

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

    const unsigned int SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;
    unsigned int depthMapFBO;
    //unsigned int depthMap;
    unsigned int depthCubemap;
    float borderColor[4] = { 1.0, 1.0, 1.0, 1.0 };

    void drawAll(Shader* shader = nullptr);

public:
    GutenbergScene();
    ~GutenbergScene();

    void addPhysics(BulletWorld *world);
    void moveStatic(float time);
    void render(const glm::mat4& projection, const Camera& camera, float SCR_WIDTH, float SCR_HEIGHT);

    Shader* pbrShader;
    Shader* simpleDepthShader;
    Shader* backgroundShader;

    // lights
    // ------
    glm::vec3 lightPositions[4] = {
        glm::vec3(-150.156f,  78.2362f, -137.485f),
        glm::vec3(-100,  38, -70),
        glm::vec3(0,0,0),
        glm::vec3(0,0,0),
    };
    glm::vec3 lightColors[4] = {
        glm::vec3(1000000.0f, 1000000.0f, 1000000.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
    };
};

#endif /* _GUTENBERG_SCENE_H */
