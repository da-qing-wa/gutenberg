cmake_minimum_required(VERSION 3.0)
project(Gutenberg_mechanism)

find_package(Bullet REQUIRED)

file(GLOB SRCFILES *.cpp)

add_library(model STATIC ${SRCFILES})
target_include_directories(model PUBLIC ${BULLET_INCLUDE_DIR})
target_compile_definitions(model PUBLIC ${BULLET_DEFINITIONS})
target_link_directories(model PUBLIC ${Bullet_DIR}/${BULLET_ROOT_DIR}/${BULLET_LIBRARY_DIRS})
target_link_libraries(model PUBLIC 
	BulletDynamics BulletCollision LinearMath)
