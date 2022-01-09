//
// Created by Jonathan on 2022-01-09.
//

#ifndef TNM084_PROCEDURAL_VILLAGE_BUILDINGSPOT_H
#define TNM084_PROCEDURAL_VILLAGE_BUILDINGSPOT_H

#include "TerrainGrid.h"

class BuildingSpot {
public:

    Building(BuildingType bt, enums::Direction dir) : type{bt}, facingDirection{dir}{};


    enums::Direction facingDirection;
    BuildingType type;
};


#endif //TNM084_PROCEDURAL_VILLAGE_BUILDINGSPOT_H
