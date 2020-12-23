#ifndef LRSTATUS_H
#define LRSTATUS_H

#include "lrMesh.h"
#include "lrCamera.h"
#include "lrTexture.h"


class lrStatus{
    public:
        int windowWidth = 800;
        int windowHeight = 800;
        lrMesh *mesh;
        lrCamera *camera;
        lrTexture *texture0;
        lrTexture *texture1;
        lrTexture *texture2;
        lrTexture *texture3;


        mat4f_t view;
        mat4f_t viewPort;
        mat4f_t projection;
        mat4f_t model;

        

        
};


#endif
