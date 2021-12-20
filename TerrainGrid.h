//
// Created by Jonathan Andersson on 2021-12-20.
//

#ifndef TNM084_PROCEDURAL_VILLAGE_TERRAINGRID_H
#define TNM084_PROCEDURAL_VILLAGE_TERRAINGRID_H
#include "common/VectorUtils3.h"
#include "noise/noise1234.h"
#include "common/LittleOBJLoader.h"


class TerrainGrid {
private:
    constexpr static int kTerrainSize = 32;
    constexpr static float kPolySize = 1.f;

    vec3 vertices[kTerrainSize * kTerrainSize];
    vec2 texCoords[kTerrainSize * kTerrainSize];
    vec3 normals[kTerrainSize * kTerrainSize];
    GLuint indices[(kTerrainSize - 1) * (kTerrainSize - 1) * 3 * 2];

public:
    TerrainGrid();

    // Make basic terrain.
    // Based on lab 3b
    void MakeTerrain();

    Model* GetModelPtr();
};


#endif //TNM084_PROCEDURAL_VILLAGE_TERRAINGRID_H
