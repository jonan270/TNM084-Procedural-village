//
// Created by Jonathan on 2022-01-07.
//

#ifndef TNM084_PROCEDURAL_VILLAGE_FORESTMAP_H
#define TNM084_PROCEDURAL_VILLAGE_FORESTMAP_H

#include <cstdlib>
#include "noise1234.h"
#include "cmath"
#include <iostream>

class ForestMap {
public:
    ForestMap(float f, int rSeed);
    int randSeed;

    // Is this 2d position forested?
    bool IsForested(float x, float y);
    bool IsTreeSpot(float x, float y);
private:
    float frequency;
    float randVal;
};


#endif //TNM084_PROCEDURAL_VILLAGE_FORESTMAP_H
