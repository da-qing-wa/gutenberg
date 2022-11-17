cmake_minimum_required(VERSION 3.0)
project(Gutenberg_mechanism)

find_package(Bullet REQUIRED)

file(GLOB SRCFILES *.cpp)

add_library(model STATIC ${SRCFILES})
target_include_directories(model PUBLIC ${BULLET_INCLUDE_DIR})
target_include_directories(model PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_definitions(model PUBLIC ${BULLET_DEFINITIONS})
