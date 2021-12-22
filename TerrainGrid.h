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


class TerrainGrid {

public:
    constexpr static int kTerrainSize = 128;
    constexpr static float kPolySize = 0.1f;
    TerrainGrid();

    // Make basic terrain.
    // Based on lab 3b
    void MakeTerrain();

    Model* GetModelPtr();
private:
    constexpr static int roadIndexWidth = 2;

    // Default color to be used for grass/vegetation
    const vec3 defaultColor = vec3{ 0.24, 0.56, 0.44 };

    // Color used to represent roads
    const vec3 roadColor = vec3{ 0.67, 0.64, 0.5 };

    vec3 vertices[kTerrainSize * kTerrainSize];
    vec2 texCoords[kTerrainSize * kTerrainSize];
    vec3 normals[kTerrainSize * kTerrainSize];
    vec3 colors[kTerrainSize * kTerrainSize];
    GLuint indices[(kTerrainSize - 1) * (kTerrainSize - 1) * 3 * 2];

    std::pair<int ,int> townSquareCenterPoint =
            std::pair<int, int>(kTerrainSize/2, kTerrainSize/2);

    bool IsInTownSquare(int x, int z);
    bool IsOnRoad(int x, int z);

};


#endif //TNM084_PROCEDURAL_VILLAGE_TERRAINGRID_H
