// Jonathan Andersson - jonan270
// The basic project setup is based on lab3b from the course.
// Note that I have made som minor changes to LittleObjLoader

#include "common/GL_utilities.h"
#include "common/LittleOBJLoader.h"

#ifdef __APPLE__
#include "common/Mac/MicroGlut.h"
#include <OpenGL/gl3.h>
// linking hint for Lightweight IDE
//uses framework Cocoa
#else
    #if defined(_WIN32)
        #include "common/Windows/MicroGlut.h"
        #include "common/Windows/include/glfw3.h"
    #endif
#endif

#include <iostream>
#include <vector>
#include "GlutCameraControls.h"
#include "TerrainGrid.h"
#include "LittleOBJExtension.h"
#include "InstancingModel.h"

mat4 projectionMatrix;

std::vector<Model*> models;
std::vector<InstancingModel> insModels;

// Reference to shader programs
GLuint phongShader;

GlutCameraControls glutCameraControls =
        GlutCameraControls(TerrainGrid::kTerrainSize, TerrainGrid::kPolySize);

constexpr int RES = 1080;

void GenerateTerrain();
int GetBuildingRotationAngle(TerrainGrid::Direction dir);

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

    GenerateTerrain();

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

    if(glutKeyIsDown('z'))
        GenerateTerrain();

    glUniformMatrix4fv(
            glGetUniformLocation(phongShader, "modelviewMatrix"),
            1, GL_TRUE, m.m);

    printError("display b4");

    for(Model* model : models)
        DrawModel(model, phongShader,
                  "inPosition",
                  "inNormal",
                  NULL,
                  "inColor");

    for(InstancingModel im : insModels) {
        im.Draw(phongShader);
    }

    printError("display");
    glutSwapBuffers();
}

void keys(unsigned char key, int x, int y) {}

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

void GenerateTerrain() {
    // Empty old model data
    models = std::vector<Model*>();
    insModels = std::vector<InstancingModel>();

    // Upload geometry to the GPU:
    TerrainGrid grid{};
    Model* terrainModel = grid.GetModelPtr();

    // Load model for well to be placed in town center
    Model* wellModel = LoadModel((char *)"../obj-models/well.obj", SetVec3(0.427, 0.317, 0.235));

    // Put in center of map
    ScaleModel(wellModel, 0.1, 0.1, 0.1);
    TranslateModel(wellModel, TerrainGrid::kPolySize * TerrainGrid::kTerrainSize / 2.0, 0,
                   TerrainGrid::kPolySize * TerrainGrid::kTerrainSize / 2.0);

    int numBuildings = (int)grid.buildingSpots.size();
    std::cout << "Number of houses generated: " << numBuildings << "\n";
    InstancingModel villageHousing = InstancingModel((char *)"../obj-models/housing.obj",
                                     SetVec3(0.427, 0.317, 0.235),
                                     numBuildings);

    int numTrees = (int)grid.treeSpots.size();
    std::cout << "Number of trees generated: " << numTrees << "\n";
    InstancingModel forest = InstancingModel((char *)"../obj-models/pineTree.obj",
                                                     SetVec3(0.227, 0.352, 0.286),
                                                     numTrees);

    int count = 0;
    for(auto bp : grid.buildingSpots) {
        villageHousing.SetTranslationOfInstance(count, bp.first);
        villageHousing.SetRotationOfInstance(count, (float)GetBuildingRotationAngle(bp.second));
        count++;
    }

    count = 0;
    for(auto tp : grid.treeSpots) {
        forest.SetTranslationOfInstance(count, tp);
        count++;
    }

    insModels.push_back(villageHousing);
    insModels.push_back(forest);

    models.push_back(terrainModel);
    models.push_back(wellModel);
    printError("generate terrain");
}

int GetBuildingRotationAngle(TerrainGrid::Direction dir) {
    int angle;
    switch(dir) {
        case TerrainGrid::south:
            angle = 90;
            break;
        case TerrainGrid::southEast:
            angle = 45;
            break;
        case TerrainGrid::east:
            angle = 0;
            break;
        case TerrainGrid::northEast:
            angle = -45;
            break;
        case TerrainGrid::north:
            angle = -90;
            break;
        case TerrainGrid::northWest:
            angle = -135;
            break;
        case TerrainGrid::west:
            angle = 180;
            break;
        case TerrainGrid::southWest:
            angle = 135;
            break;
    }
    return angle;
}
