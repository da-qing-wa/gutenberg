#include <iostream>
#include <iomanip>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>
#include <memory>
#ifdef __linux__
#include <sys/stat.h>
#include <sys/time.h>
#endif

#include "BulletWorld.h"
#include "GutenbergScene.h"
#include "learnopengl/camera.h"

// #define OFFLINE_RENDERING

#ifdef OFFLINE_RENDERING
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <pthread.h>
#endif

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;
Camera camera(glm::vec3(0.0f, 500.0f, 800.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW - 15, PITCH-30);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

#ifndef OFFLINE_RENDERING
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
#endif

#ifdef OFFLINE_RENDERING
struct saveFrameData
{
	const GLsizei bufSize = 3 * SCR_WIDTH * SCR_HEIGHT;
	std::vector<char> buffer = std::vector<char>(bufSize);
	std::string fname;
};

void* saveFrameTask(void* arg)
{
	saveFrameData *targ = (saveFrameData*)arg;

	stbi_flip_vertically_on_write(true);
	stbi_write_jpg(targ->fname.c_str(), SCR_WIDTH, SCR_HEIGHT, 3, targ->buffer.data(), 3 * SCR_WIDTH);

	delete targ;
	return 0;
}
#endif

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

	std::cout << glGetString(GL_VERSION) << std::endl;
	glEnable(GL_DEPTH_TEST);

	BulletWorld *mWorld = new BulletWorld();

	GutenbergScene *mScene = new GutenbergScene();
	mScene->addPhysics(mWorld);
	
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);

#ifndef OFFLINE_RENDERING
	float lastFrame = getTimeSecs();
	float startFrame = lastFrame;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = getTimeSecs();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mScene->moveStatic(currentFrame - startFrame);

		mWorld->step(currentFrame - lastFrame);

		// update the camera
		//UpdateCamera();

		// render the scene
		mScene->render(projection, camera);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

		lastFrame = currentFrame;
	}
#else

#define FRAMERATE 60

	const float totalLength = 15.0f;
	const float dt = 1.0f / (FRAMERATE);
	const int frameCount = (int)(totalLength / dt) + 1;
	
	std::string path("./frames/");
	std::time_t ptime = std::time(nullptr);
#ifdef __linux__
	std::string ptimestr = std::to_string(ptime);
	int direrr = mkdir(ptimestr.c_str(), 0777);
	if (direrr == 0)
		path = "./" + ptimestr + '/';
#endif

#define THREAD_CNT 32
	pthread_t ths[THREAD_CNT] {};

	for (int i=0; i<frameCount; i++) {
		float t = i * dt;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mScene->moveStatic(t);

		if (i > 0)
			mWorld->step(dt);

		// update the camera
		//UpdateCamera();

		// render the scene
		mScene->render(projection, camera);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
		saveFrameData *dat = new saveFrameData();
		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glReadBuffer(GL_FRONT);
		glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, dat->buffer.data());
		int tid = (i % THREAD_CNT);
		if (ths[tid] != 0) {
			pthread_join(ths[tid], NULL);
		}
		std::stringstream fname;
		fname << path << std::setw(5) << std::setfill('0') << i << ".jpg";
		dat->fname = fname.str();
		pthread_create(&(ths[tid]), NULL, saveFrameTask, dat);
		std::cout << i << '/' << frameCount << std::endl;
	}

	for (int tid=0; tid<THREAD_CNT; tid++)
		if (ths[tid] != 0) {
			pthread_join(ths[tid], NULL);
		}

	std::cout << "Generating MP4..." << std::endl;
    std::array<char, 128> buffer;
    std::string result;
	char cmd[128] {};
	sprintf(cmd, "ffmpeg -framerate %d -pattern_type glob -i '%s*.jpg' -c:v libx264 -pix_fmt yuv420p %ld.mp4",
		FRAMERATE, path.c_str(), ptime);
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
	std::cout << result << std::endl;
	std::cout << "Generate MP4 DONE" << std::endl;

	// ffmpeg -framerate 10 -pattern_type glob -i './frames/*.jpg' out.mp4

#endif

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	delete mWorld;
	delete mScene;
	glfwTerminate();

}
