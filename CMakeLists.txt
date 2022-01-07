cmake_minimum_required(VERSION 3.20)
project(TNM084_Procedural_village)

set(CMAKE_CXX_STANDARD 14)


set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/common)

if(MACOS)
    set(OS_INCLUDE_DIR ${COMMON_INCLUDE_DIR}/Mac)
    set(MICROGLUTSOURCE ${OS_INCLUDE_DIR}/MicroGlut.m})
elseif(WIN32)
    set(OS_INCLUDE_DIR ${COMMON_INCLUDE_DIR}/Windows)
    set(MICROGLUTSOURCE ${OS_INCLUDE_DIR}/MicroGlut.c})
endif()


set(NOISE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/noise)

set(SOURCES main.cpp
        ${OS_INCLUDE_DIR}/MicroGlut.h
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
        TerrainGrid.h LittleOBJExtension.h LittleOBJExtension.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(FindPkgConfig)
find_package(glfw3 REQUIRED)

add_executable(TNM084_Procedural_village ${SOURCES})
if(MACOS)
    target_sources(TNM084_Procedural_village
            PRIVATE ${OS_INCLUDE_DIR}/MicroGlut.m)
    elseif(WIN32)
    target_sources(TNM084_Procedural_village
            PUBLIC ${OS_INCLUDE_DIR}/glew.h
            PRIVATE ${OS_INCLUDE_DIR}/glew.c
            PRIVATE ${OS_INCLUDE_DIR}/MicroGlut.c)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NOISE_INCLUDE_DIR})

include_directories(${GLFW3_INCLUDE_DIR})

if(MACOS)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL -framework Cocoa")
else()
    target_link_libraries(${PROJECT_NAME} OpenGL::GL)
endif()

