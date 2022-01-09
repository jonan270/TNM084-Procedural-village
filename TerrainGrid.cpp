#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp" // Yes, rand works. Thank you compiler

//
// Created by Jonathan Andersson on 2021-12-20.
//

#include "TerrainGrid.h"

TerrainGrid::TerrainGrid() {
    // Base random seed on current time
    randomSeed = (int)time(nullptr);
    srand(randomSeed);

    MakeTerrain();
    MakeRoads();
    MakeForest();


    /*
    for (int x = 0; x < kTerrainSize; x++) {
        for (int z = 0; z < kTerrainSize; z++) {
            int ix = GetArrIndex(x,z);
            if(occupied[ix])
                colors[ix] = SetVec3(0,1,0);
        }
    }
     */

}

void TerrainGrid::MakeTerrain() {
    for (int x = 0; x < kTerrainSize; x++) {
        for (int z = 0; z < kTerrainSize; z++) {
            int ix = GetArrIndex(x, z);
            float y = GetYNoiseValue(x, z);

            vertices[ix] = SetVec3((float)x * kPolySize, y, (float)z * kPolySize);
            texCoords[ix] = SetVec2((float)x, (float)z);

            vec3 terrainColor = defaultColor;

            // Town center placed in center of grid
            if(IsInTownSquare(x,z)) {
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
            vec3 p1 = SetVec3((float)x + 1, GetYNoiseValue(x + 1, z + 1), (float)z + 1);
            vec3 p2 = SetVec3((float)x + 1, GetYNoiseValue(x + 1, z - 1), (float)z - 1);
            vec3 p3 = SetVec3((float)x - 1, GetYNoiseValue(x - 1, z - 1), (float)z - 1);
            vec3 p4 = SetVec3((float)x - 1, GetYNoiseValue(x - 1, z + 1), (float)z + 1);

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
        int startX = townSquareCenterPoint.first - townSquareWidth;
        int startZ = townSquareCenterPoint.second - townSquareWidth;

        int endX = townSquareCenterPoint.first + townSquareWidth;
        int endZ = townSquareCenterPoint.second + townSquareWidth;

        int x, z;

        enums::Direction dir = RandDirection4();
        switch (dir) {
            case enums::Direction::south:
                //std::cout << "South\n";
                x = endX;
                z = rand() % (endZ - startZ + 1) + startZ;
                break;
            case enums::Direction::east:
                //std::cout << "East\n";
                x = rand() % (endX - startX + 1) + startX;
                z = endZ;
                break;
            case enums::Direction::north:
                //std::cout << "North\n";
                x = startX;
                z = rand() % (endZ - startZ + 1) + startZ;
                break;
            case enums::Direction::west:
                //std::cout << "West\n";
                x = rand() % (endX - startX + 1) + startX;
                z = startZ;
                break;
            case enums::Direction::southEast:
            case enums::Direction::northEast:
            case enums::Direction::northWest:
            case enums::Direction::southWest:
                // This only happens if I do something stupid.
                assert(false);
        }
        MakeRoadFrom(x, z, dir);
        // DEBUG: Mark the starting points of roads.
        //colors[GetArrIndex(x,z)] = SetVec3(1,0,0);
    }

}

void TerrainGrid::MakeRoadFrom(int x, int z, enums::Direction startDirection) {
    // Number of allowed directions for a given direction
    const int N_ALLOWED_DIR = 3;

    int countX = x;
    int countZ = z;

    /*
     Get an allowed random direction,
     disallowed directions are those
     heading opposite to the general
     start direction of a given road.

     After testing, I conclude that
     more realistic roads are generated
     when only allowing 1 step left/right
     from the general direction
    */
    enums::Direction allowed[N_ALLOWED_DIR] = {
            startDirection,
            RightFrom(startDirection),
            //RightFrom(RightFrom(startDirection)),
            LeftFrom(startDirection),
            //LeftFrom(LeftFrom(startDirection))
    };

    // How long is total road length?
    int distTotal = 0;

    // How long has road continued
    // in one direction?
    int distStraight = 0;

    // How long has road been going
    // without branching?
    int distBranch = 0;

    enums::Direction newDir = startDirection;
    while(true) {
        // Break if edge of map is reached
        if(!IsValidIndex(countX, countZ)) break;

        if (distStraight > straightLen) {
            int randDirInt;

            // Do not go in the opposite direction
            do
                randDirInt = rand() % N_ALLOWED_DIR; // 0-2
            while(allowed[randDirInt] == OppositeFrom(newDir));
            newDir = allowed[randDirInt];

            distStraight = 0;
        }

        // Set to occupied
        occupied[GetArrIndex(countX,countZ)] = true;

        DrawRoadAroundIdx(countX, countZ, newDir);
        auto p = GetNextIndexFrom(countX, countZ, newDir);
        int px = p.first;
        int pz = p.second;

        // We have encountered a crossing road! Break.
        if(IsValidIndex(px, pz) && occupied[GetArrIndex(px, pz)]) break;

        countX = px;
        countZ = pz;

        if(ShouldMakeBranch(branchProb, distBranch)) {
            // Either split of somewhere else or find a spot
            // for a building
            if(rand() % 2 == 0) {
                vec3 location = vertices[GetArrIndex(countX, countZ)];
                buildingSpots.emplace_back(location, newDir);

                // Set area around house to occupied
                for(int bx = countX - houseClearance; bx < countX + houseClearance; bx ++) {
                    for(int bz = countZ - houseClearance; bz < countZ + houseClearance; bz ++)
                        if(IsValidIndex(bx,bz)) occupied[GetArrIndex(bx,bz)] = true;
                }
                break;
            }

            // 2 directional branching
            enums::Direction branchDir = rand() % 2 == 0 ?
                    LeftFrom(newDir) : RightFrom(newDir);

            // 4 directional branching
            /*
            Direction branchDir;
            switch(rand() % 4) {
                case 0:
                    branchDir = LeftFrom(newDir);
                    break;
                case 1:
                    branchDir = RightFrom(newDir);
                    break;
                case 2:
                    branchDir = LeftFrom(LeftFrom(newDir));
                    break;
                case 3:
                    branchDir = RightFrom(RightFrom(newDir));
                    break;
                default: assert(false);
            }
            */

            MakeRoadFrom(countX, countZ, branchDir);
            distBranch = 0;
        }

        DrawRoadAroundIdx(countX, countZ, newDir);
        distStraight++;
        distBranch++;
        distTotal++;
    }
}

void TerrainGrid::MakeForest() {
    ForestMap forestMap{0.1 * kPolySize};
    for (int x = 0; x < kTerrainSize; x++) {
        for (int z = 0; z < kTerrainSize; z++) {
            // Get correct index
            int ix = GetArrIndex(x,z);

            // Check if a tree should be placed here
            if(forestMap.IsForested(x,z) && !occupied[ix] && !IsRoad(x,z))
                treeSpots.push_back(vertices[ix]);
        }
    }
}

bool TerrainGrid::IsInTownSquare(int x, int z) const {
    return (x >= townSquareCenterPoint.first - townSquareWidth &&
            x <= townSquareCenterPoint.first + townSquareWidth &&
            z >= townSquareCenterPoint.second - townSquareWidth &&
            z <= townSquareCenterPoint.second + townSquareWidth);
}

bool TerrainGrid::IsRoad(int x, int z) {
    vec3 cl = colors[GetArrIndex(x,z)];

    return
        cl.x == roadColor.x &&
        cl.y == roadColor.y &&
        cl.z == roadColor.z;
}

int TerrainGrid::GetArrIndex(int x, int z) {
    return z * kTerrainSize + x;
}

float TerrainGrid::GetYNoiseValue(int x, int z) {
    float randVal = (rand() % 100)/100.0;
    return kPolySize * noise2(kPolySize * (float)x + 0.23f * randVal,
                              kPolySize * (float)z + 0.22f * randVal);
}

enums::Direction TerrainGrid::RandDirection4() {
    int randDirInt = rand() % 4; // 0-3
    switch (randDirInt) {
        case 0: return enums::Direction::south;
        case 1: return enums::Direction::east;
        case 2: return enums::Direction::north;
        case 3: return enums::Direction::west;
        default: assert(false);
    }
}

enums::Direction TerrainGrid::RightFrom(enums::Direction current) {
    switch (current) {
        case enums::Direction::south:
            return enums::Direction::southEast;
        case enums::Direction::southEast:
            return enums::Direction::east;
        case enums::Direction::east:
            return enums::Direction::northEast;
        case enums::Direction::northEast:
            return enums::Direction::north;
        case enums::Direction::north:
            return enums::Direction::northWest;
        case enums::Direction::northWest:
            return enums::Direction::west;
        case enums::Direction::west:
            return enums::Direction::southWest;
        case enums::Direction::southWest:
            return enums::Direction::south;
    }
}

enums::Direction TerrainGrid::LeftFrom(enums::Direction current) {
    switch (current) {
        case enums::Direction::south:
            return enums::Direction::southWest;
        case enums::Direction::southEast:
            return enums::Direction::south;
        case enums::Direction::east:
            return enums::Direction::southEast;
        case enums::Direction::northEast:
            return enums::Direction::east;
        case enums::Direction::north:
            return enums::Direction::northEast;
        case enums::Direction::northWest:
            return enums::Direction::north;
        case enums::Direction::west:
            return enums::Direction::northWest;
        case enums::Direction::southWest:
            return enums::Direction::west;
    }
}

enums::Direction TerrainGrid::OppositeFrom(enums::Direction current) {
    // The lazy readable way:
    //return LeftFrom(LeftFrom(LeftFrom(LeftFrom(current))));

    // The efficient less readable way:
    switch(current) {
        case enums::Direction::south:
            return enums::Direction::north;
        case enums::Direction::southEast:
            return enums::Direction::northWest;
        case enums::Direction::east:
            return enums::Direction::west;
        case enums::Direction::northEast:
            return enums::Direction::southWest;
        case enums::Direction::north:
            return enums::Direction::south;
        case enums::Direction::northWest:
            return enums::Direction::southEast;
        case enums::Direction::west:
            return enums::Direction::east;
        case enums::Direction::southWest:
            return enums::Direction::northEast;
    }
}

std::pair<int, int>
TerrainGrid::GetNextIndexFrom(int x, int z, enums::Direction nextDir) {
    switch (nextDir) {
        case enums::Direction::south:
            return {x + 1, z};
        case enums::Direction::southEast:
            return {x + 1, z + 1};
        case enums::Direction::east:
            return {x, z + 1};
        case enums::Direction::northEast:
            return {x - 1, z + 1};
        case enums::Direction::north:
            return {x - 1, z};
        case enums::Direction::northWest:
            return {x - 1, z - 1};
        case enums::Direction::west:
            return {x, z - 1};
        case enums::Direction::southWest:
            return {x + 1, z - 1};
    }
}

void TerrainGrid::DrawRoadAroundIdx(int x, int z, enums::Direction current) {
    // Set to occupied
    //occupied[GetArrIndex(x,z)] = true;

    // This rather complicated switch sets colors
    // around (x,z) to form a road
    switch (current) {
        case enums::Direction::south:
        case enums::Direction::north:
        case enums::Direction::west:
        case enums::Direction::east:
            for(int i = x - (roadIndexWidth - 1) / 2;
                i <= x + (roadIndexWidth - 1) / 2 &&
                i > 0 && i < kTerrainSize;
                i++)
                for(int j = z - (roadIndexWidth - 1) / 2;
                    j <= z + (roadIndexWidth - 1) / 2 &&
                    j > 0 && j < kTerrainSize;
                    j++)
                    colors[GetArrIndex(i, j)] = roadColor;
            break;
        case enums::Direction::southEast:
        case enums::Direction::southWest:
        case enums::Direction::northEast:
        case enums::Direction::northWest:
            for(int i = x - (roadIndexWidth - 1)/2;
                i <= x + (roadIndexWidth - 1)/2 &&
                i > 0 && i < kTerrainSize;
                i++)
                for(int j = z - (roadIndexWidth - 1)/2;
                    j <= z + (roadIndexWidth - 1)/2 &&
                    j > 0 && z < kTerrainSize;
                    j++)
                    colors[GetArrIndex(i,j)] = roadColor;
            break;
    }
}

TerrainGrid::~TerrainGrid() {
    delete[] vertices;
    delete[] texCoords;
    delete[] normals;
    delete[] colors;
    delete[] indices;
    delete[] occupied;
}

bool TerrainGrid::ShouldMakeBranch(float probability, int currentLength) {
    float roll = (float)(rand() % 100 + 1) / 100.f;
    return (currentLength > minBranchDist) && (roll <= probability);
}

bool TerrainGrid::IsValidIndex(int x, int z) {
    return (x < kTerrainSize && z < kTerrainSize) &&
           (x >= 0 && z >= 0);
}

#pragma clang diagnostic pop