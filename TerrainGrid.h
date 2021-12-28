//
// Created by Jonathan Andersson on 2021-12-20.
//

#ifndef TNM084_PROCEDURAL_VILLAGE_TERRAINGRID_H
#define TNM084_PROCEDURAL_VILLAGE_TERRAINGRID_H

#include "common/VectorUtils3.h"
#include "noise/noise1234.h"
#include "common/LittleOBJLoader.h"
#include "common/GL_utilities.h"
#include <tuple>
#include <cstdlib> // srand makes use of random seed
#include <time.h>

#include <iostream>


class TerrainGrid {

public:

    // The different directions where a
    // road might turn
    enum Direction {
        south,
        southEast,
        east,
        northEast,
        north,
        northWest,
        west,
        southWest
    };
    constexpr static int kTerrainSize = 128;
    constexpr static float kPolySize = 0.1f;
    TerrainGrid();

    Model* GetModelPtr();

private:
    constexpr static int roadIndexWidth = 2;
    constexpr static int townSquareWidth = 3 * roadIndexWidth;
    constexpr static int branchPoints = 3;
    constexpr static unsigned int randomSeed = 113;

    // Default color to be used for grass/vegetation
    const vec3 defaultColor = vec3{ 0.24, 0.56, 0.44 };

    // Color used to represent roads
    const vec3 roadColor = vec3{ 0.67, 0.64, 0.5 };

    vec3 vertices[kTerrainSize * kTerrainSize]; // Vertex position
    vec2 texCoords[kTerrainSize * kTerrainSize];
    vec3 normals[kTerrainSize * kTerrainSize];
    vec3 colors[kTerrainSize * kTerrainSize];
    GLuint indices[(kTerrainSize - 1) * (kTerrainSize - 1) * 3 * 2];

    std::pair<int ,int> townSquareCenterPoint =
            std::pair<int, int>(kTerrainSize/2, kTerrainSize/2);

    // Make basic terrain.
    // Based on lab 3b
    void MakeTerrain();

    // Make road network extending
    // from town square.
    void MakeRoadFrom(int x, int z, Direction startDirection);
    void MakeRoads();

    bool IsInTownSquare(int x, int z) const;
    bool IsOnRoad(int x, int z) const;

    // Get the position value
    // obtained from noise function
    static float GetYNoiseValue(int x, int z);

    // Access the correct array index
    // given grid indices for x and z
    static int GetArrIndex(int x, int z);

    // Return south, east, north, or
    // west at random.
    Direction RandDirection4();

    // Return the direction 1 step to the
    // right from current
    Direction RightFrom(Direction current) const;

    // Return the direction 1 step to the
    // left from current
    Direction LeftFrom(Direction current) const;

    std::pair<int,int> getNextIndexFrom(std::pair<int,int> currentIdx, Direction nextDir) const;
};


#endif //TNM084_PROCEDURAL_VILLAGE_TERRAINGRID_H
