// The basic project setup is based on lab3b from the course

#ifdef __APPLE__

#include <OpenGL/gl3.h>
// linking hint for Lightweight IDE
//uses framework Cocoa
#endif

#include "common/GL_utilities.h"
#include "common/Mac/MicroGlut.h"
#include "common/VectorUtils3.h"
#include "common/LittleOBJLoader.h"
#include "common/LoadTGA.h"
#include "noise/noise1234.h"
#include <math.h>
#include "GlutCameraControls.h"

mat4 projectionMatrix;
Model *floormodel;

// Reference to shader programs
GLuint phongShader;

#define kTerrainSize 32
#define kPolySize 1.0

// Terrain data. To be intialized in MakeTerrain or in the shader
vec3 vertices[kTerrainSize * kTerrainSize];
vec2 texCoords[kTerrainSize * kTerrainSize];
vec3 normals[kTerrainSize * kTerrainSize];
GLuint indices[(kTerrainSize - 1) * (kTerrainSize - 1) * 3 * 2];

GlutCameraControls glutCameraControls = GlutCameraControls(kTerrainSize, kPolySize);

// These are considered unsafe, but with most C code, write with caution.
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

void MakeTerrain() {
    // TO DO: This is where your terrain generation goes if on CPU.
    for (int x = 0; x < kTerrainSize; x++)
        for (int z = 0; z < kTerrainSize; z++) {
            int ix = z * kTerrainSize + x;
            float y = noise2(x + 0.23, z + 0.22);

            vertices[ix] = SetVec3(x * kPolySize, y, z * kPolySize);
            texCoords[ix] = SetVec2(x, z);
            //normals[ix] = SetVec3(0,1,0);
        }

    // Make indices
    // You don't need to change this.
    for (int x = 0; x < kTerrainSize - 1; x++)
        for (int z = 0; z < kTerrainSize - 1; z++) {
            // Quad count
            int q = (z * (kTerrainSize - 1)) + (x);
            // Polyon count = q * 2
            // Indices
            indices[q * 2 * 3] = x + z * kTerrainSize; // top left
            indices[q * 2 * 3 + 1] = x + 1 + z * kTerrainSize;
            indices[q * 2 * 3 + 2] = x + (z + 1) * kTerrainSize;
            indices[q * 2 * 3 + 3] = x + 1 + z * kTerrainSize;
            indices[q * 2 * 3 + 4] = x + 1 + (z + 1) * kTerrainSize;
            indices[q * 2 * 3 + 5] = x + (z + 1) * kTerrainSize;
        }

    // Make normal vectors
    // TO DO: This is where you calculate normal vectors
    for (int x = 0; x < kTerrainSize; x++)
        for (int z = 0; z < kTerrainSize; z++) {
            //normals[z * kTerrainSize + x] = SetVec3(0,1,0);

            vec3 p1 = SetVec3(x + 1, noise2(x + 1 + 0.23, z + 1 + 0.22), z + 1);
            vec3 p2 = SetVec3(x + 1, noise2(x + 1 + 0.23, z - 1 + 0.22), z - 1);
            vec3 p3 = SetVec3(x - 1, noise2(x - 1 + 0.23, z - 1 + 0.22), z - 1);
            vec3 p4 = SetVec3(x - 1, noise2(x - 1 + 0.23, z + 1 + 0.22), z + 1);

            vec3 v1 = VectorSub(p1, p3);
            vec3 v2 = VectorSub(p2, p4);

            vec3 normal = CrossProduct(v1, v2);
            normals[z * kTerrainSize + x] = normal;
            //printf("(%f, %f, %f) \n", normal.x, normal.y, normal.z);
        }
}

void init(void) {
    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    printError("GL inits");

    projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 300.0);

    // Load and compile shader
    phongShader = loadShaders("../shaders/phong.vert", "../shaders/phong.frag");
    printError("init shader");

    // Upload geometry to the GPU:
    MakeTerrain();
    floormodel = LoadDataToModel(vertices, normals, texCoords, NULL,
                                 indices, kTerrainSize * kTerrainSize, (kTerrainSize - 1) * (kTerrainSize - 1) * 2 * 3);

    printError("LoadDataToModel");

    // Important! The shader we upload to must be active!
    glUseProgram(phongShader);
    glUniformMatrix4fv(glGetUniformLocation(phongShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

    //glUniform1i(glGetUniformLocation(phongShader, "tex"), 0); // Texture unit 0

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    printError("init arrays");

}

void display(void) {

    // Floor
    glUseProgram(phongShader);
    mat4 m = glutCameraControls.UpdateWorldMatrix();
    glUniformMatrix4fv(glGetUniformLocation(phongShader, "modelviewMatrix"), 1, GL_TRUE, m.m);
    DrawModel(floormodel, phongShader, "inPosition", "inNormal", "inTexCoord");

    printError("display");

    glutSwapBuffers();
}

void keys(unsigned char key, int x, int y) {
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(1080, 1080);
    glutCreateWindow("Lab 3b");
    glutRepeatingTimer(20);
    glutDisplayFunc(display);
    glutKeyboardFunc(keys);
    init();
    glutMainLoop();
}
