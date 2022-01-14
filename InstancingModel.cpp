//
// Created by Jonathan on 2022-01-07.
//

#include "InstancingModel.h"

InstancingModel::InstancingModel(const char *filePath, vec3 color, int numInstances) {
    m = LoadInstancingModel(filePath, color, numInstances);
}

void InstancingModel::Draw(GLuint program) {
    DrawModelInstanced(m, program,
                       "inPosition",
                       "inNormal",
                       NULL,
                       "inColor",
                       "inTranslation",
                       "inAngle",
                       m->numInstances);
}

void InstancingModel::SetTranslationOfInstance(unsigned int instanceNum, vec3 translation) {
    m->instanceTranslationArray[instanceNum] = translation;
    ReloadModelData(m);
}

void InstancingModel::SetRotationOfInstance(unsigned int instanceNum, float angle) {
    m->instanceRotationArray[instanceNum] = angle;
    ReloadModelData(m);
}
