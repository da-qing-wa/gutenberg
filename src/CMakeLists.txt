cmake_minimum_required(VERSION 3.0)
project(Gutenberg_mechanism)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(Bullet REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(model)
add_subdirectory(learnopengl)
add_subdirectory(shader)

file(GLOB SRCFILES *.cpp)
set(SRCFILES
	${CMAKE_SOURCE_DIR}/include/glad/glad.c
	${CMAKE_SOURCE_DIR}/include/stb_image.cpp
	${SRCFILES} 
)

add_executable(${CMAKE_PROJECT_NAME} ${SRCFILES})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${BULLET_INCLUDE_DIRS})
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC ${BULLET_DEFINITIONS})
target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC ${BULLET_LIBRARY_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC 
	model shader learnopengl 
	BulletDynamics BulletCollision LinearMath assimp pthread
	GL glfw ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY} ${CMAKE_DL_LIBS})
