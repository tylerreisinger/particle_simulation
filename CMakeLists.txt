cmake_minimum_required(VERSION 2.6)
project(ParticleSwarmTextureGeneration)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
include_directories("src")

if(ENABLE_TRACING)
    add_definitions(-DTRACING)
endif()
if(ENABLE_COLOR_CUI)
    add_definitions(-DCUI)
endif()

list(APPEND CMAKE_CXX_FLAGS "-std=c++14")

add_executable(pstg ${SOURCES} ${TRACING_SOURCES} ${CUI_SOURCES})
