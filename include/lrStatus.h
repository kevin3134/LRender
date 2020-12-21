#ifndef LRSTATUS_H
#define LRSTATUS_H

#include "lrMesh.h"
#include "lrCamera.h"

class lrStatus{
    public:
        int windowWidth = 800;
        int windowHeight = 800;
        lrMesh *mesh;
        lrCamera *camera;

        mat4f_t view;
        mat4f_t viewPort;
        mat4f_t projection;
        mat4f_t model;
};


#endif
