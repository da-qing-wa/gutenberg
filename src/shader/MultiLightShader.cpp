#include "MultiLightShader.h"

MultiLightShader::MultiLightShader() : 
    shader(
        "./resources/shader/multi_lights/multi_lights.vs",
        "./resources/shader/multi_lights/multi_lights.fs"
    )
{
    shader.use();
	// directional light
	shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	// point light 1
	shader.setVec3("pointLights[0].position", glm::vec3(1.0f, 0.3f, 0.0f));
	shader.setVec3("pointLights[0].ambient", 0.5f, 0.5f, 0.5f);
	shader.setVec3("pointLights[0].diffuse", 1.0f, 0.0f, 0.0f);
	shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[0].constant", 1.0f);
	shader.setFloat("pointLights[0].linear", 0.09f);
	shader.setFloat("pointLights[0].quadratic", 0.032f);
	// point light 2
	shader.setVec3("pointLights[1].position", glm::vec3(-1.0f, 0.3f, 0.0f));
	shader.setVec3("pointLights[1].ambient", 0.5f, 0.5f, 0.5f);
	shader.setVec3("pointLights[1].diffuse", 0.0f, 1.0f, 0.0f);
	shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[1].constant", 1.0f);
	shader.setFloat("pointLights[1].linear", 0.09f);
	shader.setFloat("pointLights[1].quadratic", 0.032f);
	// point light 3
	shader.setVec3("pointLights[2].position", glm::vec3(0.0f, 0.3f, 1.0f));
	shader.setVec3("pointLights[2].ambient", 0.5f, 0.5f, 0.5f);
	shader.setVec3("pointLights[2].diffuse", 0.0f, 0.0f, 1.0f);
	shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[2].constant", 1.0f);
	shader.setFloat("pointLights[2].linear", 0.09f);
	shader.setFloat("pointLights[2].quadratic", 0.032f);
	// point light 4
	shader.setVec3("pointLights[3].position", glm::vec3(0.0f, 0.3f, -1.0f));
	shader.setVec3("pointLights[3].ambient", 0.5f, 0.5f, 0.5f);
	shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[3].constant", 1.0f);
	shader.setFloat("pointLights[3].linear", 0.09f);
	shader.setFloat("pointLights[3].quadratic", 0.032f);

	// spotLight
	// shader.setVec3("spotLight.position", camera.Position);
	// shader.setVec3("spotLight.direction", camera.Front);
	shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("spotLight.constant", 1.0f);
	shader.setFloat("spotLight.linear", 0.09f);
	shader.setFloat("spotLight.quadratic", 0.032f);
	shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

	shader.setFloat("material.shininess", 32.0f);
}

void MultiLightShader::use(const glm::vec3& cameraPosition, const glm::vec3& cameraFront) 
{
    shader.use();
    shader.setVec3("spotLight.position", cameraPosition);
    shader.setVec3("spotLight.direction", cameraFront);
	shader.setVec3("viewPos", cameraPosition);
}

Shader* MultiLightShader::getShader()
{
	return &shader;
}
