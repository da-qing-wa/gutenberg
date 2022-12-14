#include "GutenbergScene.h"
#include <iostream>

//unsigned int loadTexture(const char* path);
void renderSphere();
void renderCube();
void renderQuad();

GutenbergScene::GutenbergScene()
{
    // shaders
    pbrShader = new Shader("./resources/shader/pbr/pbr.vs", "./resources/shader/pbr/pbr.fs");
    equirectangularToCubemapShader = new Shader("./resources/shader/equirectangularToCubemap/cubemap.vs", "./resources/shader/equirectangularToCubemap/equirectangular_to_cubemap.fs");
    irradianceShader = new Shader("./resources/shader/irradiance/cubemap.vs", "./resources/shader/irradiance/irradiance_convolution.fs");
    prefilterShader = new Shader("./resources/shader/prefilter/cubemap.vs", "./resources/shader/prefilter/prefilter.fs");
    brdfShader = new Shader("./resources/shader/brdf/brdf.vs", "./resources/shader/brdf/brdf.fs");
    backgroundShader = new Shader("./resources/shader/background/background.vs", "./resources/shader/background/background.fs");
    //simpleDepthShader = new Shader("./resources/shader/shadow_depth/shadow_depth.vs", "./resources/shader/shadow_depth/shadow_depth.fs");
    simpleDepthShader = new Shader("./resources/shader/shadow_depth/shadow_depth.vs", "./resources/shader/shadow_depth/shadow_depth.fs", "./resources/shader/shadow_depth/shadow_depth.gs");

    pbrShader->use();
    pbrShader->setInt("irradianceMap", 0);
    pbrShader->setInt("prefilterMap", 1);
    pbrShader->setInt("brdfLUT", 2);
    pbrShader->setInt("albedoMap", 3);
    pbrShader->setInt("normalMap", 4);
    pbrShader->setInt("metallicMap", 5);
    pbrShader->setInt("roughnessMap", 6);
    pbrShader->setInt("aoMap", 7);

    backgroundShader->use();
    backgroundShader->setInt("environmentMap", 0);

    // configure depth map FBO
    // -----------------------
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (GLuint i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // Attach cubemap as depth map FBO's color buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //pbr
    {
        // pbr: setup framebuffer
        // ----------------------
        unsigned int captureFBO;
        unsigned int captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

        // pbr: load the HDR environment map
        // ---------------------------------
        stbi_set_flip_vertically_on_load(true);
        int width, height, nrComponents;
        float* data = stbi_loadf("resources/textures/hdr/Mt-Washington-Gold-Room_Ref.hdr", &width, &height, &nrComponents, 0);

        unsigned int hdrTexture;
        if (data)
        {
            glGenTextures(1, &hdrTexture);
            glBindTexture(GL_TEXTURE_2D, hdrTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load HDR image." << std::endl;
        }

        // pbr: setup cubemap to render to and attach to framebuffer
        // ---------------------------------------------------------

        glGenTextures(1, &envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
        // ----------------------------------------------------------------------------------------------
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        // pbr: convert HDR equirectangular environment map to cubemap equivalent
        // ----------------------------------------------------------------------
        equirectangularToCubemapShader->use();
        equirectangularToCubemapShader->setInt("equirectangularMap", 0);
        equirectangularToCubemapShader->setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);

        glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            equirectangularToCubemapShader->setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
        // --------------------------------------------------------------------------------
        glGenTextures(1, &irradianceMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

        // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
        // -----------------------------------------------------------------------------
        irradianceShader->use();
        irradianceShader->setInt("environmentMap", 0);
        irradianceShader->setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            irradianceShader->setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
        // --------------------------------------------------------------------------------
        glGenTextures(1, &prefilterMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
        // ----------------------------------------------------------------------------------------------------
        prefilterShader->use();
        prefilterShader->setInt("environmentMap", 0);
        prefilterShader->setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        unsigned int maxMipLevels = 5;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            prefilterShader->setFloat("roughness", roughness);
            for (unsigned int i = 0; i < 6; ++i)
            {
                prefilterShader->setMat4("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                renderCube();
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // pbr: generate a 2D LUT from the BRDF equations used.
        // ----------------------------------------------------
        glGenTextures(1, &brdfLUTTexture);

        // pre-allocate enough memory for the LUT texture.
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
        // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

        glViewport(0, 0, 512, 512);
        brdfShader->use();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderQuad();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    // precious BALL
    btScalar ball_rad = 8.32f;
    ball = new MovingObject("ball", 8.0, pbrShader, btVector3(ball_rad / 2.54, ball_rad / 2.54, ball_rad / 2.54), btVector3(-15, 137.0, -135.0), 0.18f);

    // objects
    //rounddesk = new StaticObject("rounddesk", pbrShader, btVector3(1, 1, 1), btVector3(-100, -784.3, 0), 0.5f);
    rounddesk = new StaticObject("rounddesk", pbrShader, btVector3(1, 1, 1), btVector3(-100, -474.3, 0), 0.5f);
    ground = new StaticObject("ground", pbrShader, btVector3(1.0, 0.1, 1.0), btVector3(-100, -23.1, 0), 0.5f);

    slide1 = new StaticObject("try7", pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(-28, 0, 0), 0.3f);
    wm_blade = new StaticObject("wm_blade", pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(95.74, 46.81, -135.0), 0.3f);
    wm_body = new StaticObject("wm_body", pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(-11.2, 0, -39), 0.3f);
    rail = new StaticObject("rail", pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(8, 7, 0), 0.3f);

    dom1 = new MovingObject("dom1", 10.0, pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(-108.33, -10.395, -0.28), 0.5f, BOX_SHAPE_PROXYTYPE);
    dom2 = new MovingObject("dom2", 20.0, pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(-123.87, -7.2869, -0.28), 0.5f, BOX_SHAPE_PROXYTYPE);
    dom3 = new MovingObject("dom3", 30.0, pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(-139.41, -4.7189, -0.28), 0.5f, BOX_SHAPE_PROXYTYPE);
    dom4 = new MovingObject("dom4", 40.0, pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(-154.95, -1.0708, -0.28), 0.5f, BOX_SHAPE_PROXYTYPE);
    dom5 = new MovingObject("dom5", 50.0, pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(-170.49, 2.3072, -0.28), 0.5f, BOX_SHAPE_PROXYTYPE);

    ball2 = new MovingObject("ball2", 30.0, pbrShader, btVector3(ball_rad / 2.54, ball_rad / 2.54, ball_rad / 2.54), btVector3(-190.0, 30.28, -0.0), 0.001f);
    table = new StaticObject("try3", pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(0, 0, 0), 0.03f);
    slide2 = new StaticObject("try6", pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(0, 0, 0), 0.03f);

    float block_friction = 10.1f, block_width = 1.0f, wall_x1 = -348, wall_dy = -6.0;
    block1 = new MovingObject("block1", 5.0, pbrShader, btVector3(block_width, 1.0, 1.0), btVector3(wall_x1, 13.475 + wall_dy, -10), block_friction);
    block2 = new MovingObject("block1", 5.0, pbrShader, btVector3(block_width, 1.0, 1.0), btVector3(wall_x1, 13.475 + wall_dy, 0), block_friction);
    block3 = new MovingObject("block1", 5.0, pbrShader, btVector3(block_width, 1.0, 1.0), btVector3(wall_x1, 13.475 + wall_dy, 10), block_friction);
    block4 = new MovingObject("block1", 5.0, pbrShader, btVector3(block_width, 1.0, 1.0), btVector3(wall_x1, 0 + wall_dy, -10), block_friction);
    block5 = new MovingObject("block1", 5.0, pbrShader, btVector3(block_width, 1.0, 1.0), btVector3(wall_x1, 0 + wall_dy, 0), block_friction);
    block6 = new MovingObject("block1", 5.0, pbrShader, btVector3(block_width, 1.0, 1.0), btVector3(wall_x1, 0 + wall_dy, 10), block_friction);
    block7 = new MovingObject("block1", 5.0, pbrShader, btVector3(block_width, 1.0, 1.0), btVector3(wall_x1, -13.475 + wall_dy, -10), block_friction);
    block8 = new MovingObject("block1", 5.0, pbrShader, btVector3(block_width, 1.0, 1.0), btVector3(wall_x1, -13.475 + wall_dy, 0), block_friction);
    block9 = new MovingObject("block1", 5.0, pbrShader, btVector3(block_width, 1.0, 1.0), btVector3(wall_x1, -13.475 + wall_dy, 10), block_friction);

    desk_lamp = new StaticObject("desk_lamp", pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(-160.79, -23.1, -140.86), 0.5f);
    clock = new StaticObject("clock", pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(0, 0, 0), 0.5f);
    lime = new StaticObject("lime", pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(0, 0, 0), 0.5f);
    potted_plant = new StaticObject("potted_plant", pbrShader, btVector3(1.0, 1.0, 1.0), btVector3(0, 0, 0), 0.5f);
}

void GutenbergScene::addPhysics(BulletWorld *world)
{
    world->addRigidBody(ball->getBody());
    world->addRigidBody(ground->getBody());
    world->addRigidBody(slide1->getBody());
    world->addRigidBody(wm_blade->getBody());
    world->addRigidBody(wm_body->getBody());
    world->addRigidBody(rail->getBody());
    world->addRigidBody(dom1->getBody());
    world->addRigidBody(dom2->getBody());
    world->addRigidBody(dom3->getBody());
    world->addRigidBody(dom4->getBody());
    world->addRigidBody(dom5->getBody());
    world->addRigidBody(ball2->getBody());
    world->addRigidBody(table->getBody());
    world->addRigidBody(slide2->getBody());
    world->addRigidBody(block1->getBody());
    world->addRigidBody(block2->getBody());
    world->addRigidBody(block3->getBody());
    world->addRigidBody(block4->getBody());
    world->addRigidBody(block5->getBody());
    world->addRigidBody(block6->getBody());
    world->addRigidBody(block7->getBody());
    world->addRigidBody(block8->getBody());
    world->addRigidBody(block9->getBody());
    world->addRigidBody(desk_lamp->getBody());
    world->addRigidBody(clock->getBody());
    world->addRigidBody(lime->getBody());
    world->addRigidBody(potted_plant->getBody());
}

GutenbergScene::~GutenbergScene()
{
    delete ball;
    delete ground;
    delete rounddesk;
    delete slide1;
    delete wm_blade;
    delete wm_body;
    delete rail;
    delete dom1;
    delete dom2;
    delete dom3;
    delete dom4;
    delete dom5;
    delete ball2;
    delete table;
    delete slide2;
    delete block1;
    delete block2;
    delete block3;
    delete block4;
    delete block5;
    delete block6;
    delete block7;
    delete block8;
    delete block9;
    delete desk_lamp;
    delete clock;
    delete lime;
    delete potted_plant;

    delete pbrShader;
    delete equirectangularToCubemapShader;
    delete irradianceShader;
    delete prefilterShader;
    delete brdfShader;
    delete backgroundShader;
    delete simpleDepthShader;
}

void GutenbergScene::moveStatic(float time)
{
    const glm::vec3 AXIS_Z = glm::vec3(0.0f, 0.0f, 1.0f);

    btTransform trans;
    glm::mat4 model;
    
    static bool exch = false;
    if (ball->getBody()->getCenterOfMassTransform().getOrigin().getY() < 13.0f && !exch) {
    //if (time > 120.0f && !exch) {
        exch = true;
         rail->getBody()->setFriction(0.0f);
         ball->getBody()->setCollisionShape(new btSphereShape(8.32f));
         ball->getBody()->setFriction(0.0f);
         ball2->getBody()->setCollisionShape(new btSphereShape(8.32f));
         ball2->getBody()->setFriction(0.0f);
    }

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(wm_blade->getOriginalLocation().getX(), wm_blade->getOriginalLocation().getY(), wm_blade->getOriginalLocation().getZ()));
    model = glm::rotate(model, glm::radians(140.0f), AXIS_Z);
    if (ball->getBody()->getCenterOfMassTransform().getOrigin().getY() < 13.0f)
        model = glm::rotate(model, glm::radians(40 * time), AXIS_Z);
    
    trans.setFromOpenGLMatrix(glm::value_ptr(model));
    wm_blade->getBody()->getMotionState()->setWorldTransform(trans);
    wm_blade->getBody()->setWorldTransform(trans);
}

void GutenbergScene::drawAll(Shader* shader)
{
    ball->draw(shader);
    //ground->draw(shader);
    rounddesk->draw(shader);
    slide1->draw(shader);
    wm_blade->draw(shader);
    wm_body->draw(shader);
    rail->draw(shader);
    dom1->draw(shader);
    dom2->draw(shader);
    dom3->draw(shader);
    dom4->draw(shader);
    dom5->draw(shader);
    ball2->draw(shader);
    table->draw(shader);
    slide2->draw(shader);
    block1->draw(shader);
    block2->draw(shader);
    block3->draw(shader);
    block4->draw(shader);
    block5->draw(shader);
    block6->draw(shader);
    block7->draw(shader);
    block8->draw(shader);
    block9->draw(shader);
    if (shader != simpleDepthShader)
        desk_lamp->draw(shader);
    clock->draw(shader);
    lime->draw(shader);
    potted_plant->draw(shader);
}

void GutenbergScene::render(const glm::mat4& projection, const Camera& camera, float SCR_WIDTH, float SCR_HEIGHT)
{
    //printf("camera:pos(%f,%f,%f),up(%f,%f,%f),YAW(%f),PITCH(%f)\n", camera.Position.x, camera.Position.y, camera.Position.z, camera.Up.x, camera.Up.y, camera.Up.z, camera.Yaw, camera.Pitch);
    // render scene, supplying the convoluted irradiance map to the final shader.
    // ------------------------------------------------------------------------------------------
    pbrShader->use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    pbrShader->setMat4("view", view);
    pbrShader->setVec3("camPos", camera.Position);
    pbrShader->setInt("depthMap", 8);
    //pbrShader->setInt("shadowMap", 20);

    // bind pre-computed IBL data
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);


    // 0. Create depth cubemap transformation matrices
    GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
    GLfloat near = 0.1f;
    GLfloat far = 500.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositions[0], lightPositions[0] + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositions[0], lightPositions[0] + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositions[0], lightPositions[0] + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositions[0], lightPositions[0] + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositions[0], lightPositions[0] + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositions[0], lightPositions[0] + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

    // 1. Render scene to depth cubemap
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    simpleDepthShader->use();
    for (GLuint i = 0; i < 6; ++i)
        glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader->ID, ("shadowMatrices[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(shadowTransforms[i]));
    simpleDepthShader->setFloat("far_plane", far);
    simpleDepthShader->setVec3("lightPos", lightPositions[0]);
    drawAll(simpleDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // reset viewport
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pbrShader->use();
    pbrShader->setFloat("far_plane", far);
    glActiveTexture(GL_TEXTURE8);
    //glActiveTexture(GL_TEXTURE20);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    drawAll(pbrShader);


    // render light source (simply re-render sphere at light positions)
    // this looks a bit off as we use the same shader, but it'll make their positions obvious and 
    // keeps the codeprint small.
    for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
    {
        glm::vec3 newPos;
        newPos = lightPositions[i];
        pbrShader->setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
        pbrShader->setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

        model = glm::mat4(1.0f);
        model = glm::translate(model, newPos);
        model = glm::scale(model, glm::vec3(3.5f));
        pbrShader->setMat4("model", model);
        //renderCube();
        //renderSphere();
    }

    // render skybox (render as last to prevent overdraw)
    backgroundShader->use();
    backgroundShader->setMat4("view", view);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
    //glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap); // display prefilter map
    renderCube();
}


// renders (and builds at first invocation) a sphere
// -------------------------------------------------
unsigned int sphereVAO = 0;
GLsizei indexCount;
void renderSphere()
{
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = static_cast<GLsizei>(indices.size());

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        unsigned int stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
             // bottom face
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             // top face
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
              1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
