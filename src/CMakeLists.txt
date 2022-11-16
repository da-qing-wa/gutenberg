cmake_minimum_required(VERSION 3.0)
project(Gutenberg_mechanism)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(Bullet REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED STATIC)
find_package(glm REQUIRED)

add_subdirectory(model)

file(GLOB SRCFILES *.cpp)
# set(SRCFILES main.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SRCFILES})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${BULLET_INCLUDE_DIR})
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC ${BULLET_DEFINITIONS})
target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC ${Bullet_DIR}/${BULLET_ROOT_DIR}/${BULLET_LIBRARY_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC 
	model 
	BulletDynamics BulletCollision LinearMath
	GL GLEW glfw ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY} ${GLEW_LIBRARY})
