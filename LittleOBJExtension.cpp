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

void DrawModelInstanced(Model *m, GLuint program,
                        char* vertexVariableName,
                        char* normalVariableName,
                        char* texCoordVariableName,
                        char* colorVariableName,
                        char* translationVariableName,
                        int count) {
    if (m != NULL) {
        GLint loc;

        glBindVertexArray(m->vao);	// Select VAO

        glBindBuffer(GL_ARRAY_BUFFER, m->vb);
        loc = glGetAttribLocation(program, vertexVariableName);
        if (loc >= 0) {
            glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(loc);
        }
        else
            fprintf(stderr, "DrawModelInstanced warning: '%s' not found in shader!\n", vertexVariableName);

        if (normalVariableName!=NULL) {
            loc = glGetAttribLocation(program, normalVariableName);
            if (loc >= 0) {
                glBindBuffer(GL_ARRAY_BUFFER, m->nb);
                glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(loc);
            }
            else
                fprintf(stderr, "DrawModelInstanced warning: '%s' not found in shader!\n",
                        normalVariableName);
        }

        // VBO for texture coordinate data NEW for 5b
        if ((m->texCoordArray != NULL)&&(texCoordVariableName != NULL)) {
            loc = glGetAttribLocation(program, texCoordVariableName);
            if (loc >= 0) {
                glBindBuffer(GL_ARRAY_BUFFER, m->tb);
                glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(loc);
            }
            else
                fprintf(stderr, "DrawModelInstanced warning: '%s' not found in shader!\n",
                        texCoordVariableName);
        }

        if (colorVariableName!=NULL) {
            loc = glGetAttribLocation(program, colorVariableName);
            if (loc >= 0) {
                glBindBuffer(GL_ARRAY_BUFFER, m->cb);
                glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(loc);
            }
            else
                fprintf(stderr, "DrawModelInstanced warning: '%s' not found in shader!\n",
                        colorVariableName);
        }

        if (translationVariableName!=NULL) {
            loc = glGetAttribLocation(program, translationVariableName);
            if (loc >= 0) {
                glBindBuffer(GL_ARRAY_BUFFER, m->isb);
                glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(loc);

                // Divisor 1 sets variable to entire model
                // rather than for each vertex in model.
                glVertexAttribDivisor(loc, 1);
            }
            else
                fprintf(stderr, "DrawModelInstanced warning: '%s' not found in shader!\n",
                        colorVariableName);
        }

        glDrawElementsInstanced(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L, count);
    }
}



