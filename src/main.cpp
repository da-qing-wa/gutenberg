#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifdef __linux__
#include <sys/time.h>
#endif

#include "BulletWorld.h"
#include "GutenbergScene.h"
#include "learnopengl/camera.h"

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;
Camera camera(glm::vec3(-4.0f, 2.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW + 45, PITCH - 10);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

float getTimeSecs()
{
#ifdef __linux__
	static bool inited = false;
	static struct timeval start;
	if (!inited) {
		inited = true;
		gettimeofday(&start, NULL);
	}
    static struct timeval stop;
    gettimeofday(&stop, NULL);
	// return  * 1000 + ((stop.tv_usec - start->tv_usec) / 1000);
    return (stop.tv_sec - start.tv_sec) + (stop.tv_usec - start.tv_usec) / 1000000.0f;
#else 
	return (float)glfwGetTime();
#endif 
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
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Gutenberg Mechanism", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	BulletWorld *mWorld = new BulletWorld();

	GutenbergScene *mScene = new GutenbergScene();
	mScene->addPhysics(mWorld);
	
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	float lastFrame = getTimeSecs();
	float startFrame = lastFrame;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = getTimeSecs();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mScene->moveStatic(currentFrame - startFrame);

		mWorld->step(currentFrame - lastFrame);
		lastFrame = currentFrame;

		// update the camera
		//UpdateCamera();

		// render the scene
		mScene->render(projection, camera);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	delete mWorld;
	delete mScene;
	glfwTerminate();

}
