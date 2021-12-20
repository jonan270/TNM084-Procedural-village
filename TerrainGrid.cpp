//
// Created by Jonathan Andersson on 2021-12-20.
//

#include "TerrainGrid.h"

TerrainGrid::TerrainGrid() {
    MakeTerrain();
}

void TerrainGrid::MakeTerrain() {
    for (int x = 0; x < kTerrainSize; x++) {
        for (int z = 0; z < kTerrainSize; z++) {
            int ix = z * kTerrainSize + x;
            float y = 0.05f * noise2(0.1f*x + 0.23, 0.1f*z + 0.22);

            vertices[ix] = SetVec3(x * kPolySize, y, z * kPolySize);
            texCoords[ix] = SetVec2(x, z);
            //normals[ix] = SetVec3(0,1,0);

            vec3 terrainColor = defaultColor;

            // Town center placed in center of grid
            if( (z >= kTerrainSize/2 - 2 * roadIndexWidth && z <= kTerrainSize/2 + 2 * roadIndexWidth) &&
                (x >= kTerrainSize/2 - 2 * roadIndexWidth && x <= kTerrainSize/2 + 2 * roadIndexWidth) ) {
                terrainColor = roadColor;
            }
            else if( z >= kTerrainSize/2 - roadIndexWidth &&
                     z <= kTerrainSize/2 + roadIndexWidth) {
                terrainColor = roadColor;
            }

            colors[ix] = terrainColor;
        }
    }

    // Make indices
    // You don't need to change this.
    for (int x = 0; x < kTerrainSize - 1; x++) {
        for (int z = 0; z < kTerrainSize - 1; z++) {
            // Quad count
            int q = (z * (kTerrainSize - 1)) + (x);
            // Polyon count = q * 2
            // Indices
            indices[q * 2 * 3] = x + z * kTerrainSize; // top left
            indices[q * 2 * 3 + 1] = x + 1 + z * kTerrainSize;
            indices[q * 2 * 3 + 2] = x + (z + 1) * kTerrainSize;
            indices[q * 2 * 3 + 3] = x + 1 + z * kTerrainSize;
            indices[q * 2 * 3 + 4] = x + 1 + (z + 1) * kTerrainSize;
            indices[q * 2 * 3 + 5] = x + (z + 1) * kTerrainSize;
        }
    }

    // Make normal vectors
    for (int x = 0; x < kTerrainSize; x++) {
        for (int z = 0; z < kTerrainSize; z++) {
            //normals[z * kTerrainSize + x] = SetVec3(0,1,0);

            vec3 p1 = SetVec3(x + 1, 0.1f * noise2(x + 1 + 0.23, z + 1 + 0.22), z + 1);
            vec3 p2 = SetVec3(x + 1, 0.1f * noise2(x + 1 + 0.23, z - 1 + 0.22), z - 1);
            vec3 p3 = SetVec3(x - 1, 0.1f * noise2(x - 1 + 0.23, z - 1 + 0.22), z - 1);
            vec3 p4 = SetVec3(x - 1, 0.1f * noise2(x - 1 + 0.23, z + 1 + 0.22), z + 1);

            vec3 v1 = VectorSub(p1, p3);
            vec3 v2 = VectorSub(p2, p4);

            vec3 normal = CrossProduct(v1, v2);
            normals[z * kTerrainSize + x] = normal;
        }
    }
}

Model *TerrainGrid::GetModelPtr() {
    Model* model = LoadDataToModel(
            vertices, normals,
            texCoords, colors,
            indices, kTerrainSize * kTerrainSize,
            (kTerrainSize - 1) * (kTerrainSize - 1) * 2 * 3);

    printError("LoadDataToModel");
    return model;
}

bool TerrainGrid::IsInTownSquare(int x, int z) {
    return (x >= townSquareCenterPoint.first - 2 * roadIndexWidth &&
            x <= townSquareCenterPoint.first + 2 * roadIndexWidth &&
            z >= townSquareCenterPoint.second - 2 * roadIndexWidth &&
            z <= townSquareCenterPoint.second + 2 * roadIndexWidth);
}

bool TerrainGrid::IsOnRoad(int x, int z) {
    return (z >= townSquareCenterPoint.second - roadIndexWidth &&
            z <= townSquareCenterPoint.second + roadIndexWidth);
}
