#ifndef _MULTI_LIGHT_SHADER_H
#define _MULTI_LIGHT_SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../learnopengl/shader.h"

class MultiLightShader {
protected:
    Shader shader;

public:
    MultiLightShader();

    void use(const glm::vec3& cameraPosition, const glm::vec3& cameraFront);
    Shader* getShader();

};

#endif /* _MULTI_LIGHT_SHADER_H */