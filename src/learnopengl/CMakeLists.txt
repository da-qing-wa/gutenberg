cmake_minimum_required(VERSION 3.0)
project(Gutenberg_mechanism)

set(SRCFILES
	mesh.cpp
	model.cpp
    shader.cpp
	${SRCFILES} 
)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(learnopengl STATIC ${SRCFILES})
