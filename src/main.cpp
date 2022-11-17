#include <iostream>

#include "BulletWorld.h"
#include "model/MovingObject.h"
#include "model/StaticObject.h"
#include "learnopengl/camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "LinearMath/btIDebugDraw.h"
//#include "GLDebugDrawer.h"

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;
Camera camera(glm::vec3(-4.0f, 2.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW + 45, PITCH - 10);

// positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(1.0f,  0.3f,  0.0f),
	glm::vec3(-1.0f, 0.3f,  0.0f),
	glm::vec3(0.0f,  0.3f,  1.0f),
	glm::vec3(0.0f,  0.3f, -1.0f),
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// set shader with multi lights
void set_multi_lights(Shader& shader, glm::vec3 pointLightPositions[], Camera& camera)
{
	// directional light
	shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	// point light 1
	shader.setVec3("pointLights[0].position", pointLightPositions[0]);
	shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[0].diffuse", 1.0f, 0.0f, 0.0f);
	shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[0].constant", 1.0f);
	shader.setFloat("pointLights[0].linear", 0.09f);
	shader.setFloat("pointLights[0].quadratic", 0.032f);
	// point light 2
	shader.setVec3("pointLights[1].position", pointLightPositions[1]);
	shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[1].diffuse", 0.0f, 1.0f, 0.0f);
	shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[1].constant", 1.0f);
	shader.setFloat("pointLights[1].linear", 0.09f);
	shader.setFloat("pointLights[1].quadratic", 0.032f);
	// point light 3
	shader.setVec3("pointLights[2].position", pointLightPositions[2]);
	shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[2].diffuse", 0.0f, 0.0f, 1.0f);
	shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[2].constant", 1.0f);
	shader.setFloat("pointLights[2].linear", 0.09f);
	shader.setFloat("pointLights[2].quadratic", 0.032f);
	// point light 4
	shader.setVec3("pointLights[3].position", pointLightPositions[3]);
	shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[3].constant", 1.0f);
	shader.setFloat("pointLights[3].linear", 0.09f);
	shader.setFloat("pointLights[3].quadratic", 0.032f);

	// spotLight
	shader.setVec3("spotLight.position", camera.Position);
	shader.setVec3("spotLight.direction", camera.Front);
	shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("spotLight.constant", 1.0f);
	shader.setFloat("spotLight.linear", 0.09f);
	shader.setFloat("spotLight.quadratic", 0.032f);
	shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
}


void UpdateCamera() 
{

}

//void BulletOpenGLApplication::DrawObj(GLInstanceGraphicsShape* glmesh) {
//}
//
//void BulletOpenGLApplication::RotateCamera(float &angle, float value) {
//	// change the value (it is passed by reference, so we
//	// can edit it here)
//	angle -= value;
//	// keep the value within bounds
//	if (angle < 0) angle += 360;
//	if (angle >= 360) angle -= 360;
//	// update the camera since we changed the angular value
//	UpdateCamera();
//}
//
//void BulletOpenGLApplication::ZoomCamera(float distance) {
//	// change the distance value
//	m_cameraDistance -= distance;
//	// prevent it from zooming in too far
//	if (m_cameraDistance < 0.1f) m_cameraDistance = 0.1f;
//	// update the camera since we changed the zoom distance
//	UpdateCamera();
//}


int main(int argc, char* argv[])
{
	// glfw: initialize and configure
// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	//stbi_set_flip_vertically_on_load(true);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	Shader mShader("./resources/shader/multi_lights/multi_lights.vs", "./resources/shader/multi_lights/multi_lights.fs");

	mShader.use();
	mShader.setVec3("viewPos", camera.Position);
	mShader.setFloat("material.shininess", 32.0f);
	mShader.setMat4("projection", projection);
	mShader.setMat4("view", view);
	set_multi_lights(mShader, pointLightPositions, camera);

	BulletWorld mWorld;

	//MovingObject ground("ground", 0, mShader, btVector3(0.01, 0.01, 0.01), btVector3(0, 0, 0));
	//mWorld.addRigidBody(ground.getBody());

	MovingObject neck_walker("neck_walker", 1.0, mShader, btVector3(0.1, 0.1, 0.1), btVector3(0, 10, 0));
	mWorld.addRigidBody(neck_walker.getBody());

	//MovingObject ZLO200("ZLO200", 1.0, mShader, btVector3(0.1, 0.1, 0.1), btVector3(0, 20, 0));
	//mWorld.addRigidBody(ZLO200.getBody());

	StaticObject triground("ZLO200", mShader, btVector3(0.1, 0.1, 0.1), btVector3(0, 0, 0));
	mWorld.addRigidBody(triground.getBody());

	

	/// Do some simulation

	///-----stepsimulation_start-----

	//todo while
		// render loop
	// -----------
	float startFrame = static_cast<float>(glfwGetTime());
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// get the time since the last iteration
		float dt = mWorld.getClock().getTimeMilliseconds();
		// reset the clock to 0
		mWorld.getClock().reset();
		// update the scene (convert ms to s)
		mWorld.step(dt/1000.0f);

		// update the camera
		//UpdateCamera();

		//// render the scene
		//RenderScene(camera);
		mWorld.mDynamicsWorld->debugDrawWorld();

		//ground.draw();
		triground.draw();
		neck_walker.draw();
		

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();


}
