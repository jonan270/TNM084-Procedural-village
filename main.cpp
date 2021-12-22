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

#include "GlutCameraControls.h"
#include "TerrainGrid.h"

mat4 projectionMatrix;
Model*terrainModel;
Model* wellPtr;

// Reference to shader programs
GLuint phongShader;

#define kTerrainSize 32
#define kPolySize 1.0

GlutCameraControls glutCameraControls = GlutCameraControls(kTerrainSize, kPolySize);

constexpr int RES = 1080;

void init() {
    // GL inits
    glClearColor(0.37, 0.5, 0.6, 1);
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
    TerrainGrid grid{};
    terrainModel = grid.GetModelPtr();

    wellPtr = LoadModel((char *)"../obj-models/well.obj", SetVec3(0.5, 0.1, 0.1));
    //wellPtr->material = terrainModel->material;

    // Important! The shader we upload to must be active!
    glUseProgram(phongShader);
    glUniformMatrix4fv(glGetUniformLocation(phongShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    printError("init arrays");

}

void display() {

    // Floor
    glUseProgram(phongShader);
    mat4 m = glutCameraControls.UpdateWorldMatrix();
    glUniformMatrix4fv(glGetUniformLocation(phongShader, "modelviewMatrix"), 1, GL_TRUE, m.m);

    DrawModel(terrainModel, phongShader, "inPosition", "inNormal", "inTexCoord", "inColor");
    DrawModel(wellPtr, phongShader, (char *)"inPosition", "inNormal", (char *)"inTexCoord", "inColor");

    printError("display");

    glutSwapBuffers();
}

void keys(unsigned char key, int x, int y) {
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(RES, RES);
    glutCreateWindow("Procedural village");
    glutRepeatingTimer(20);
    glutDisplayFunc(display);
    glutKeyboardFunc(keys);
    init();
    glutMainLoop();
}
