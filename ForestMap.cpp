//
// Created by Jonathan on 2022-01-07.
//

#include "ForestMap.h"

ForestMap::ForestMap(float f) : frequency{f} {}
ForestMap::ForestMap() : frequency{1.f} {}

int ForestMap::IsForested(float x, float y) {
    // Makes use of unseeded rand, ensures same
    // return value every time.
    float randVal = (rand() % 100)/100.0;
    float res = noise2(frequency * (float)x + 0.23f * randVal,
                       frequency * (float)y + 0.22f * randVal);
    return res > 0.3;
}
