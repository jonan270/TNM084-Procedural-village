//
// Created by Jonathan Andersson on 2022-01-05.
//

#ifndef TNM084_PROCEDURAL_VILLAGE_LITTLEOBJEXTENSION_H
#define TNM084_PROCEDURAL_VILLAGE_LITTLEOBJEXTENSION_H
#include "common/LittleOBJLoader.h"

// Translate the model m along coordinate variables
// x, y and z.
void TranslateModel(Model* m, float x, float y, float z);

// Translate the model m along the y-axis
// according to angle
void RotateModelY(Model* m, float angle);

#endif //TNM084_PROCEDURAL_VILLAGE_LITTLEOBJEXTENSION_H
