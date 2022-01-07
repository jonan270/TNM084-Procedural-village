//
// Created by Jonathan on 2022-01-07.
//

#ifndef TNM084_PROCEDURAL_VILLAGE_FORESTMAP_H
#define TNM084_PROCEDURAL_VILLAGE_FORESTMAP_H

#include <cstdlib>
#include "noise1234.h"

class ForestMap {
public:
    ForestMap();
    ForestMap(float f);

    // Is this 2d position forested?
    IsForested(float x, float y);
private:
    float frequency;
};


#endif //TNM084_PROCEDURAL_VILLAGE_FORESTMAP_H
