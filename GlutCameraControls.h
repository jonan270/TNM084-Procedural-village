#ifndef TNM084_PROCEDURAL_VILLAGE_GLUTCAMERACONTROLS_H
#define TNM084_PROCEDURAL_VILLAGE_GLUTCAMERACONTROLS_H
//#include "common/VectorUtils3.h"

// Based on lab3 but moved to improve readability of main
class GlutCameraControls {
private:
    vec3 campos;
    vec3 forward;
    vec3 up;

public:

    GlutCameraControls(int kTSize, float kPsize) {
        campos = {kTSize * kPsize / 4, 1.5, kTSize * kPsize / 4};
        forward = {8, 0, 8};
        up = {0, 1, 0};
    }

    mat4 UpdateWorldMatrix() {
        int i, j;

        printError("pre display");

        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 m;

        if (glutKeyIsDown('a'))
            forward = MultMat3Vec3(mat4tomat3(Ry(0.03)), forward);
        if (glutKeyIsDown('d'))
            forward = MultMat3Vec3(mat4tomat3(Ry(-0.03)), forward);
        if (glutKeyIsDown('w'))
            campos = VectorAdd(campos, ScalarMult(forward, 0.01));
        if (glutKeyIsDown('s'))
            campos = VectorSub(campos, ScalarMult(forward, 0.01));
        if (glutKeyIsDown('q')) {
            vec3 side = CrossProduct(forward, SetVector(0, 1, 0));
            campos = VectorSub(campos, ScalarMult(side, 0.01));
        }
        if (glutKeyIsDown('e')) {
            vec3 side = CrossProduct(forward, SetVector(0, 1, 0));
            campos = VectorAdd(campos, ScalarMult(side, 0.01));
        }

        // Move up/down
        if (glutKeyIsDown('x'))
            campos = VectorAdd(campos, ScalarMult(SetVector(0, 1, 0), 0.01));
        if (glutKeyIsDown('c'))
            campos = VectorSub(campos, ScalarMult(SetVector(0, 1, 0), 0.01));

        // NOTE: Looking up and down is done by making a side vector and rotation around arbitrary axis!
        if (glutKeyIsDown('+')) {
            vec3 side = CrossProduct(forward, SetVector(0, 1, 0));
            mat4 m = ArbRotate(side, 0.05);
            forward = MultMat3Vec3(mat4tomat3(m), forward);
        }
        if (glutKeyIsDown('-')) {
            vec3 side = CrossProduct(forward, SetVector(0, 1, 0));
            mat4 m = ArbRotate(side, -0.05);
            forward = MultMat3Vec3(mat4tomat3(m), forward);
        }
        return lookAtv(campos, VectorAdd(campos, forward), up);
    }
};


#endif //TNM084_PROCEDURAL_VILLAGE_GLUTCAMERACONTROLS_H
