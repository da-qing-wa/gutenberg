cmake_minimum_required(VERSION 3.0)
project(Gutenberg_mechanism)

find_package(assimp REQUIRED)

file(GLOB SRCFILES *.cpp)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(learnopengl STATIC ${SRCFILES})
