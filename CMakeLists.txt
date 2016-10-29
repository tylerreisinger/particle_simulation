cmake_minimum_required(VERSION 2.6)
project(ParticleSwarmTextureGeneration)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
include_directories("src")

list(APPEND CMAKE_CXX_FLAGS "-std=c++14")

add_executable(pstg ${SOURCES})
