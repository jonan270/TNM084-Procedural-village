cmake_minimum_required(VERSION 3.20)
project(TNM084_Procedural_village)

set(CMAKE_CXX_STANDARD 14)


set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/common)


set(NOISE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/noise)

set(SOURCES main.cpp
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
        TerrainGrid.h
        LittleOBJExtension.h
        LittleOBJExtension.cpp
        InstancingModel.h
        InstancingModel.cpp
        BiomeMap.h
        BiomeMap.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(TNM084_Procedural_village ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NOISE_INCLUDE_DIR})

if(APPLE)
    set(OS_INCLUDE_DIR ${COMMON_INCLUDE_DIR}/Mac)
    target_sources(TNM084_Procedural_village
            PUBLIC ${OS_INCLUDE_DIR}/MicroGlut.h
            PRIVATE ${OS_INCLUDE_DIR}/MicroGlut.m)

    target_link_libraries(${PROJECT_NAME} "-framework OpenGL -framework Cocoa")
elseif(WIN32)
    set(OS_INCLUDE_DIR ${COMMON_INCLUDE_DIR}/Windows)
    target_sources(TNM084_Procedural_village
            PUBLIC ${OS_INCLUDE_DIR}/glew.h
            PUBLIC ${OS_INCLUDE_DIR}/MicroGlut.h
            PRIVATE ${OS_INCLUDE_DIR}/glew.c
            PRIVATE ${OS_INCLUDE_DIR}/MicroGlut.c)

    target_link_libraries(${PROJECT_NAME} OpenGL::GL)
endif()

