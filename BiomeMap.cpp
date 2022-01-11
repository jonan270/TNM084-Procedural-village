//
// Created by Jonathan on 2022-01-07.
//

#include "BiomeMap.h"

BiomeMap::BiomeMap(float f, int rSeed) : frequency{f} {
    srand(rSeed);
    randSeed = rSeed;
    randVal = (rand() % 100 + 1.f)/100.f; // Between 0-1
    std::cout << "Randval: " << randVal << "\n";
}

bool BiomeMap::IsForested(float x, float y) {
    // "Big noise pattern", draws blobs of
    // forest.
    float largeNoise = std::abs(noise2(0.5*frequency * (float)x + 0.23f * randVal,
                       0.5*frequency * (float)y + 0.22f * randVal));
    return largeNoise > 0.2;

}

bool BiomeMap::IsTreeSpot(float x, float y) {
    // Small noise pattern, place individual
    // trees.
    float smallNoise = std::abs(noise2(1000.0 * frequency * (float)x + 0.23f,
                                       1000.0 * frequency * (float)y + 0.22f));

    //std::cout << "Randval = " << randVal << ", smallNoise = " << smallNoise << "\n";

    return IsForested(x,y) && smallNoise > 0.4;
}

bool BiomeMap::IsLake(float x, float y) {
    return LakeNoise(x, y , 0.35);
}

bool BiomeMap::IsBeach(float x, float y) {
    return LakeNoise(x, y, 0.34);
}

bool BiomeMap::LakeNoise(float x, float y, float gate) {
    float largeNoise = std::abs(noise2(0.2*frequency * (float)x + 0.773f * randVal,
                                       0.2*frequency * (float)y + 0.627f * randVal));
    return largeNoise > gate - 0.1;
}
