cmake_minimum_required(VERSION 3.0)
project(Gutenberg_mechanism)

add_subdirectory(src)
file(CREATE_LINK ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/src/resources SYMBOLIC)
