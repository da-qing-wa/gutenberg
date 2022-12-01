#ifndef _SKYBOX_SHADER_H
#define _SKYBOX_SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "../learnopengl/shader.h"
#include "../learnopengl/camera.h"

class SkyboxShader {
protected:
    Shader shader;

public:
    SkyboxShader();
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;
    void use(const glm::mat4& projection, const Camera& camera);
    Shader* getShader();
    unsigned int loadCubemap(std::vector<std::string> faces);
    
};

#endif /* _SKYBOX_SHADER_H */