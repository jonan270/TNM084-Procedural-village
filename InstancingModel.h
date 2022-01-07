//
// Created by Jonathan on 2022-01-07.
//

#ifndef TNM084_PROCEDURAL_VILLAGE_INSTANCINGMODEL_H
#define TNM084_PROCEDURAL_VILLAGE_INSTANCINGMODEL_H

#include "LittleOBJExtension.h"


class InstancingModel {
public:
    void Draw(GLuint program);
    InstancingModel(const char* filePath, vec3 color, int numInstances);

    void SetTranslationOfInstance(unsigned int instanceNum, vec3 translation);
    void SetRotationOfInstance(unsigned int instanceNum, float angle);

private:
    Model* m;
};


#endif //TNM084_PROCEDURAL_VILLAGE_INSTANCINGMODEL_H
