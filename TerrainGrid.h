//
// Created by Jonathan Andersson on 2021-12-20.
//

#ifndef TNM084_PROCEDURAL_VILLAGE_TERRAINGRID_H
#define TNM084_PROCEDURAL_VILLAGE_TERRAINGRID_H

#include "common/VectorUtils3.h"
#include "noise/noise1234.h"
#include "common/LittleOBJLoader.h"
#include "common/GL_utilities.h"
#include <vector>
#include <tuple>
#include <cstdlib> // srand makes use of random seed
#include <ctime>
#include <cassert>

#include <iostream>


class TerrainGrid {

public:

    // The different directions where a
    // road might be headed.
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


    // The class stores appropriate locations
    // for buildings.
    std::vector<std::pair<vec3,Direction>>
    buildingSpots;

    constexpr static int kTerrainSize = 512;
    constexpr static float kPolySize = 0.1f;

    TerrainGrid();
    ~TerrainGrid();

    Model* GetModelPtr();

private:
    constexpr static int roadIndexWidth = 3;
    constexpr static int townSquareWidth = 3 * roadIndexWidth;

    constexpr static int minBranchDist = 150;
    constexpr static int branchPoints = 4;

    constexpr static int straightLen = 3;
    int randomSeed;

    // Default color to be used for grass/vegetation
    //const vec3 defaultColor = vec3{ 0.24, 0.56, 0.44 }; // Old
    const vec3 defaultColor = vec3{ 0.22,0.37,0.31};

    // Color used to represent roads
    //const vec3 roadColor = vec3{ 0.67, 0.64, 0.5 }; // Old
    const vec3 roadColor = vec3{ 0.45,0.41,0.35};

    // Due to the large memory requirements, arrays
    // need to be stored on heap memory.
    vec3* vertices  = new vec3[kTerrainSize * kTerrainSize];
    vec2* texCoords = new vec2[kTerrainSize * kTerrainSize];
    vec3* normals   = new vec3[kTerrainSize * kTerrainSize];
    vec3* colors    = new vec3[kTerrainSize * kTerrainSize];

    // Is there a road or building present?
    bool* occupied  = new bool[kTerrainSize * kTerrainSize]{false};
    GLuint* indices = new GLuint[(kTerrainSize - 1) * (kTerrainSize - 1) * 3 * 2];

    std::pair<int ,int> townSquareCenterPoint =
            std::pair<int, int>(kTerrainSize/2, kTerrainSize/2);

    // Make basic terrain.
    // Based on lab 3b
    void MakeTerrain();

    // Make road network extending
    // from town square.
    void MakeRoadFrom(int x, int z, Direction startDirection, int maxDist = -1);
    void MakeRoads();

    void DrawRoadAroundIdx(int x, int z, Direction current);

    // Is gridslot (x,z) in the town square?
    bool IsInTownSquare(int x, int z) const;

    bool IsRoad(int x, int z);

    bool IsValidIndex(int x, int z);

    // Get the position value
    // obtained from noise function
    static float GetYNoiseValue(int x, int z);

    // Access the correct array index
    // given grid indices for x and z
    static int GetArrIndex(int x, int z);

    // Return south, east, north, or
    // west at random.
    static Direction RandDirection4();

    // Get an allowed random direction,
    // disallowed directions are those
    // heading opposite to the general
    // start direction of a given road.

    // Return the direction 1 step to the
    // right from current
    static Direction RightFrom(Direction current);

    // Return the direction 1 step to the
    // left from current
    static Direction LeftFrom(Direction current);

    // Return the direction opposite
    // from current. I.E current = north => return south
    static Direction OppositeFrom(Direction current);

    // Get the connecting index when moving
    // in the provided Direction nextDir
    static std::pair<int,int>
    GetNextIndexFrom(int x, int z, Direction nextDir);

    static bool ShouldMakeBranch(float probability, int currentLenght);
};


#endif //TNM084_PROCEDURAL_VILLAGE_TERRAINGRID_H
