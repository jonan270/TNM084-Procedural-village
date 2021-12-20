//
// Created by Jonathan Andersson on 2021-12-20.
//

#include "TerrainGrid.h"

TerrainGrid::TerrainGrid() {
    MakeTerrain();
}

void TerrainGrid::MakeTerrain() {
    for (int x = 0; x < kTerrainSize; x++)
        for (int z = 0; z < kTerrainSize; z++) {
            int ix = z * kTerrainSize + x;
            float y = noise2(x + 0.23, z + 0.22);

            vertices[ix] = SetVec3(x * kPolySize, y, z * kPolySize);
            texCoords[ix] = SetVec2(x, z);
            //normals[ix] = SetVec3(0,1,0);
        }

    // Make indices
    // You don't need to change this.
    for (int x = 0; x < kTerrainSize - 1; x++)
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

    // Make normal vectors
    for (int x = 0; x < kTerrainSize; x++)
        for (int z = 0; z < kTerrainSize; z++) {
            //normals[z * kTerrainSize + x] = SetVec3(0,1,0);

            vec3 p1 = SetVec3(x + 1, noise2(x + 1 + 0.23, z + 1 + 0.22), z + 1);
            vec3 p2 = SetVec3(x + 1, noise2(x + 1 + 0.23, z - 1 + 0.22), z - 1);
            vec3 p3 = SetVec3(x - 1, noise2(x - 1 + 0.23, z - 1 + 0.22), z - 1);
            vec3 p4 = SetVec3(x - 1, noise2(x - 1 + 0.23, z + 1 + 0.22), z + 1);

            vec3 v1 = VectorSub(p1, p3);
            vec3 v2 = VectorSub(p2, p4);

            vec3 normal = CrossProduct(v1, v2);
            normals[z * kTerrainSize + x] = normal;
        }
}

Model *TerrainGrid::GetModelPtr() {
    return LoadDataToModel(
            vertices, normals,
            texCoords, NULL,
            indices, kTerrainSize * kTerrainSize,
            (kTerrainSize - 1) * (kTerrainSize - 1) * 2 * 3);
}
