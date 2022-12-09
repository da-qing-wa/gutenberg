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
#include "learnopengl/camera.h"

// #define OFFLINE_RENDERING
#include "GutenbergScene.h"

#ifdef OFFLINE_RENDERING
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <pthread.h>
#endif

// settings
const unsigned int SCR_WIDTH = 2160;
const unsigned int SCR_HEIGHT = 1440;

Camera camera(glm::vec3(110.9f, 164.2f, 361.2f), glm::vec3(0.0f, 1.0f, 0.0f), YAW - 25.92, PITCH - 24.91);

float deltaTime = 0.0f; // time between current frame and last frame 
float lastFrame = 0.0f;
float lastX = SCR_WIDTH / 2.0f; float lastY = SCR_HEIGHT / 2.0f; bool firstMouse = true;
float last = 0.0f;
const float PI = 3.14159265359;

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void mouse_callback(GLFWwindow* window, double xpos, double ypos); 
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); 
void processInput(GLFWwindow* window);

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
void UpdateCamera(float t, float a, float b)
{
 glm::vec3 center(-110.0f, 150.0f, -80.2f);
 float init_theta = 72.93 / 180 * PI;
 camera.Position = glm::vec3(a*cos(t+ init_theta)+ center.x, center.y,b*sin(t+ init_theta)+ center.z);
 if (camera.Position.x < center.x)
 {
  camera.Yaw = atan((camera.Position.z- center.z) / (camera.Position.x- center.x)) * 180 / PI;
 }
 else if (camera.Position.z > center.z)
 {
  camera.Yaw = atan((camera.Position.z - center.z) / (camera.Position.x - center.x)) * 180 / PI - 180;
 }
 else
 {
  camera.Yaw = atan((camera.Position.z - center.z) / (camera.Position.x - center.x)) * 180 / PI + 180;
 }
 camera.updateCameraVectors();
}

void UpdateCamera2(float t)
{
	const glm::vec3 kInitPos(-111.0f, 200.0f, -59.2f);
	const glm::vec3 kUp(0.0f, 1.0f, 0.0f);
	const float kYaw = YAW + 200.9f;
	const float kPitch = PITCH - 6.8f;

	camera.Position = glm::vec3(
		kInitPos.x,
		kInitPos.y + 0.0 * t,
		kInitPos.z
	);
	camera.Yaw = kYaw;
	camera.Pitch = kPitch;

	camera.updateCameraVectors();
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


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
#ifndef OFFLINE_RENDERING
	glfwSetCursorPosCallback(window, mouse_callback); 
	glfwSetScrollCallback(window, scroll_callback); // tell GLFW to capture our mouse 

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;
	glEnable(GL_DEPTH_TEST);
	// set depth function to less than AND equal for skybox depth trick.
	glDepthFunc(GL_LEQUAL);
	// enable seamless cubemap sampling for lower mip levels in the pre-filter map.
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	BulletWorld *mWorld = new BulletWorld();

	GutenbergScene *mScene = new GutenbergScene();
	mScene->addPhysics(mWorld);
	
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
	glm::mat4 view = camera.GetViewMatrix();

	// initialize static shader uniforms before rendering
	// --------------------------------------------------
	mScene->pbrShader->use();
	mScene->pbrShader->setMat4("projection", projection);
	mScene->backgroundShader->use();
	mScene->backgroundShader->setMat4("projection", projection);

	// then before rendering, configure the viewport to the original framebuffer's screen dimensions
	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);


#ifndef OFFLINE_RENDERING
	float lastFrame = getTimeSecs();
	float startFrame = lastFrame;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = getTimeSecs();

		float current = static_cast<float>(glfwGetTime()); 
		deltaTime = current - last; 
		last = current;

		//lastFrame = currentFrame;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);

		mScene->moveStatic(currentFrame - startFrame);

		mWorld->step(currentFrame - lastFrame);

		// update the camera
		// float view_a = 500, view_b = 300;
		// UpdateCamera(0.003 * currentFrame, view_a, view_b);
		// UpdateCamera2(currentFrame);

		// render the scene
		mScene->render(projection, camera, SCR_WIDTH, SCR_HEIGHT);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
		// cout << camera.Position.x << ' ' << camera.Position.y << ' ' << camera.Position.z << ' ' << camera.Yaw - YAW << ' ' << camera.Pitch - PITCH << endl;

		lastFrame = currentFrame;
	}
#else

#define FRAMERATE 60

	const float totalLength = 100.0f;
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
		float view_a = 210, view_b = 210;
		UpdateCamera(0.05 * t, view_a, view_b);
		// UpdateCamera2(t);

		// render the scene
		mScene->render(projection, camera, SCR_WIDTH, SCR_HEIGHT);

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
		FRAMERATE * 4, path.c_str(), ptime);
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

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

