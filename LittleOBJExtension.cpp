//
// Created by Jonathan Andersson on 2022-01-05.
//
#include "LittleOBJExtension.h"
void TranslateModel(Model* m, float x, float y, float z) {
    for (long int i = 0; i < m->numVertices; i++) {
        m->vertexArray[i].x += x;
        m->vertexArray[i].y += y;
        m->vertexArray[i].z += z;
    }
    ReloadModelData(m);
}

void RotateModelY(Model* m, float angle) {
    for (long int i = 0; i < m->numVertices; i++) {
        vec4 current = vec3tovec4(m->vertexArray[i]);
        m->vertexArray[i] = vec4tovec3(Ry(angle) * current);
    }
}

