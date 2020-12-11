#ifndef LRCAMERA_H
#define LRCAMERA_H

#include "lrMath.h"

class lrCamera{
    public:
        lrCamera();
        ~lrCamera();

        void setCenter(vec3f_t c){center = c;}
        void setEye(vec3f_t e){eye = e;}

        mat4f_t lrLookAt();
        vec3f_t lrFront();
    private:
        vec3f_t eye;
        vec3f_t center;
        vec3f_t up;
};


#endif