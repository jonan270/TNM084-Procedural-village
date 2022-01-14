//
// Created by Jonathan on 2022-01-07.
//

#ifndef TNM084_PROCEDURAL_VILLAGE_BIOMEMAP_H
#define TNM084_PROCEDURAL_VILLAGE_BIOMEMAP_H

#include <cstdlib>
#include "noise1234.h"
#include "cmath"
#include <iostream>

class BiomeMap {
public:
    BiomeMap(float f, int rSeed);
    int randSeed;

    // Is this 2d position forested?
    bool IsForested(float x, float y);
    bool IsLake(float x, float y);
    bool IsBeach(float x, float y);

    bool IsTreeSpot(float x, float y);
private:
    float frequency;
    float randVal;

    bool LakeNoise(float x, float y, float gate);
};


#endif //TNM084_PROCEDURAL_VILLAGE_BIOMEMAP_H
