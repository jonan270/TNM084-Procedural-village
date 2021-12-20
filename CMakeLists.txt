cmake_minimum_required(VERSION 3.20)
project(TNM084_Procedural_village)

set(CMAKE_CXX_STANDARD 14)

set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/common)
set(NOISE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/noise)

set(SOURCES main.cpp
        ${COMMON_INCLUDE_DIR}/Mac/MicroGlut.h
        ${COMMON_INCLUDE_DIR}/Mac/MicroGlut.m
        ${COMMON_INCLUDE_DIR}/GL_utilities.h
        ${COMMON_INCLUDE_DIR}/GL_utilities.c
        ${COMMON_INCLUDE_DIR}/VectorUtils3.h
        ${COMMON_INCLUDE_DIR}/VectorUtils3.c
        ${COMMON_INCLUDE_DIR}/LoadTGA.h
        ${COMMON_INCLUDE_DIR}/LoadTGA.c
        ${COMMON_INCLUDE_DIR}/LittleOBJLoader.h
        ${COMMON_INCLUDE_DIR}/LittleOBJLoader.c
        ${NOISE_INCLUDE_DIR}/noise1234.h
        ${NOISE_INCLUDE_DIR}/noise1234.c
        GlutCameraControls.h
        TerrainGrid.cpp
        TerrainGrid.h)

find_package(OpenGL REQUIRED)

add_executable(TNM084_Procedural_village ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NOISE_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} "-framework OpenGL -framework Cocoa")

