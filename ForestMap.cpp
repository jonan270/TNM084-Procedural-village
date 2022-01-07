//
// Created by Jonathan on 2022-01-07.
//

#include "ForestMap.h"

ForestMap::ForestMap(float f) : frequency{f} {}
ForestMap::ForestMap() : frequency{1.f} {}

int ForestMap::IsForested(float x, float y) {
    // Makes use of unseeded rand, ensures same
    // return value every time.

    // "Big noise pattern", draws blobs of
    // forest.
    float randVal = (rand() % 100)/100.0;
    float largeNoise = noise2(frequency * (float)x + 0.23f * randVal,
                       frequency * (float)y + 0.22f * randVal);
    if(largeNoise < 0.3) return false;

    // Small noise pattern, place individual
    // trees.
    float smallNoise = noise2(0.001*frequency * (float)x + 0.23f * randVal,
                              0.001*frequency * (float)y + 0.22f * randVal);
    return smallNoise > 0.15;
}
