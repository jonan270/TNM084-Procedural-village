//
// Created by Jonathan Andersson on 2021-12-20.
//

#include "TerrainGrid.h"

TerrainGrid::TerrainGrid() {
    //srand(randomSeed);
    srand(time(nullptr));
    MakeTerrain();
    MakeRoads();
}

void TerrainGrid::MakeTerrain() {
    auto currentRoadPos = std::pair<int,int>(0,0);
    Direction dir = Direction::southEast;

    for (int x = 0; x < kTerrainSize; x++) {
        for (int z = 0; z < kTerrainSize; z++) {
            int ix = GetArrIndex(x, z);
            float y = GetYNoiseValue(x, z);

            vertices[ix] = SetVec3(x * kPolySize, y, z * kPolySize);
            texCoords[ix] = SetVec2(x, z);
            //normals[ix] = SetVec3(0,1,0);

            vec3 terrainColor = defaultColor;

            // Town center placed in center of grid
            if(IsInTownSquare(x,z)) {
                terrainColor = roadColor;
            }
            //
            //else if(IsOnRoad(x,z)) {
            //    terrainColor = roadColor;
            //}

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

            vec3 p1 = SetVec3(x + 1, GetYNoiseValue(x, z), z + 1);
            vec3 p2 = SetVec3(x + 1, GetYNoiseValue(x, z), z - 1);
            vec3 p3 = SetVec3(x - 1, GetYNoiseValue(x, z), z - 1);
            vec3 p4 = SetVec3(x - 1, GetYNoiseValue(x, z), z + 1);

            vec3 v1 = VectorSub(p1, p3);
            vec3 v2 = VectorSub(p2, p4);

            vec3 normal = CrossProduct(v1, v2);
            normals[GetArrIndex(x,z)] = normal;
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

void TerrainGrid::MakeRoads() {
    for(int i = 0; i < branchPoints; i++) {
        std::cout << i << "\n";
        /*
        // Town square is a square grid
        int startX = townSquareCenterPoint.first - townSquareWidth;
        int startZ = townSquareCenterPoint.second - townSquareWidth;

        int endX = townSquareCenterPoint.first + townSquareWidth;
        int endZ = townSquareCenterPoint.second + townSquareWidth;

        // Make branches extending from towncenter border at random.

        // Get random x index between [startX, endX]
        int randX = rand() % (endX - startX + 1) + startX;
        int z;
        // Follow along z and place at border.
        if(randX == startX) {
            z = startZ;
        }
        else if(randX == endX) {
            z = endZ;
        }
        else {
            z = rand() % 2 == 0 ? startZ : endZ;
        }
        std::cout << "Randomly picked index was: ("
        << randX << ", " << z << ")\n";

        colors[GetArrIndex(randX,z)] = SetVec3(1,0,0);
         */

        // Pick a random 4 way direction to find
        // along which grid border to place
        // connecting road.

        int startX = townSquareCenterPoint.first - townSquareWidth;
        int startZ = townSquareCenterPoint.second - townSquareWidth;

        int endX = townSquareCenterPoint.first + townSquareWidth;
        int endZ = townSquareCenterPoint.second + townSquareWidth;

        int x{}, z{};

        Direction dir = RandDirection4();
        switch (dir) {
            case Direction::south:
                std::cout << "South\n";
                x = endX;
                z = rand() % (endZ - startZ + 1) + startZ;
                break;
            case Direction::east:
                std::cout << "East\n";
                x = rand() % (endX - startX + 1) + startX;
                z = endZ;
                break;
            case Direction::north:
                std::cout << "North\n";
                x = startX;
                z = rand() % (endZ - startZ + 1) + startZ;
                break;
            case Direction::west:
                std::cout << "West\n";
                x = rand() % (endX - startX + 1) + startX;
                z = startZ;
                break;
        }
        MakeRoadFrom(x, z, dir);
        colors[GetArrIndex(x,z)] = SetVec3(1,0,0);
    }

}

void TerrainGrid::MakeRoadFrom(int x, int z, Direction startDirection) {
    Direction currentDirection = startDirection;

    // Number of allowed directions for a given direction
    const int N_ALLOWED_DIR = 5;

    int countX = x;
    int countZ = z;

    // Get an allowed random direction,
    // disallowed directions are those
    // heading opposite to the general
    // start direction of a given road.
    Direction allowed[N_ALLOWED_DIR] = {
            startDirection,
            RightFrom(startDirection),
            RightFrom(RightFrom(startDirection)),
            LeftFrom(startDirection),
            LeftFrom(LeftFrom(startDirection))
    };

    while(true) {
        // Break when edge of map is reached
        if( (countX >= kTerrainSize || countZ >= kTerrainSize) ||
            (countX <= 0 || countZ <= 0))
            break;

        int randDirInt = rand() % N_ALLOWED_DIR; // 0-4
        Direction newDir = allowed[randDirInt];

        auto p = getNextIndexFrom({countX, countZ}, newDir);
        currentDirection = newDir;
        countX = p.first;
        countZ = p.second;
        colors[GetArrIndex(countX, countZ)] = roadColor;
    }

    /*
    switch (startDirection) {
        case Direction::south:

            //for(int i = x; i < kTerrainSize; i++) {
            //    colors[GetArrIndex(i,z)] = roadColor;
            //}

            while(true) {
                // Break when edge of map is reached
                if( (countX >= kTerrainSize || countZ >= kTerrainSize) ||
                    (countX <= 0 || countZ <= 0))
                    break;

                int randDirInt = rand() % N_ALLOWED_DIR; // 0-4
                Direction newDir = allowed[randDirInt];

                auto p = getNextIndexFrom({countX, countZ}, newDir);
                currentDirection = newDir;
                countX = p.first;
                countZ = p.second;
                colors[GetArrIndex(countX, countZ)] = roadColor;
            }
            break;
        case Direction::east:
            for(int i = z; i < kTerrainSize; i++) {
                colors[GetArrIndex(x,i)] = roadColor;
            }
            break;
        case Direction::north:
            for(int i = x; i > 0; i--) {
                colors[GetArrIndex(i,z)] = roadColor;
            }
            break;
        case Direction::west:
            for(int i = z; i > 0; i--) {
                colors[GetArrIndex(x,i)] = roadColor;
            }
            break;
    }
    */
}

bool TerrainGrid::IsInTownSquare(int x, int z) const {
    return (x >= townSquareCenterPoint.first - townSquareWidth &&
            x <= townSquareCenterPoint.first + townSquareWidth &&
            z >= townSquareCenterPoint.second - townSquareWidth &&
            z <= townSquareCenterPoint.second + townSquareWidth);
}

bool TerrainGrid::IsOnRoad(int x, int z) const {
    return (z >= townSquareCenterPoint.second - roadIndexWidth &&
            z <= townSquareCenterPoint.second + roadIndexWidth);
}

int TerrainGrid::GetArrIndex(int x, int z) {
    return z * kTerrainSize + x;
}

float TerrainGrid::GetYNoiseValue(int x, int z) {
    return 0.75f * noise2(0.1f * (float)x + 0.23f, 0.1f * (float)z + 0.22f);;
}

TerrainGrid::Direction TerrainGrid::RandDirection4() {
    int randDirInt = rand() % 4; // 0-3
    switch (randDirInt) {
        case 0: return Direction::south;
        case 1: return Direction::east;
        case 2: return Direction::north;
        case 3: return Direction::west;
    }
}

TerrainGrid::Direction TerrainGrid::RightFrom(TerrainGrid::Direction current) const {
    switch (current) {
        case Direction::south:
            return Direction::southEast;
        case Direction::southEast:
            return Direction::east;
        case Direction::east:
            return Direction::northEast;
        case Direction::northEast:
            return Direction::north;
        case Direction::north:
            return Direction::northWest;
        case Direction::northWest:
            return Direction::west;
        case Direction::west:
            return Direction::southWest;
        case Direction::southWest:
            return Direction::south;
    }
}

TerrainGrid::Direction TerrainGrid::LeftFrom(TerrainGrid::Direction current) const {
    switch (current) {
        case Direction::south:
            return Direction::southWest;
        case Direction::southEast:
            return Direction::south;
        case Direction::east:
            return Direction::southEast;
        case Direction::northEast:
            return Direction::east;
        case Direction::north:
            return Direction::northEast;
        case Direction::northWest:
            return Direction::north;
        case Direction::west:
            return Direction::northWest;
        case Direction::southWest:
            return Direction::west;
    }
}

std::pair<int, int>
TerrainGrid::getNextIndexFrom(std::pair<int, int> currentIdx, TerrainGrid::Direction nextDir) const {
    int x = currentIdx.first;
    int z = currentIdx.second;
    switch (nextDir) {
        case Direction::south:
            return {x + 1, z};
        case Direction::southEast:
            return {x + 1, z + 1};
        case Direction::east:
            return {x, z + 1};
        case Direction::northEast:
            return {x - 1, z + 1};
        case Direction::north:
            return {x - 1, z};
        case Direction::northWest:
            return {x - 1, z - 1};
        case Direction::west:
            return {x, z - 1};
        case Direction::southWest:
            return {x + 1, z - 1};
    }
}
