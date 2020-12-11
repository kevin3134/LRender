#include "../include/lrCamera.h"


lrCamera::lrCamera(){
    up = vec3f_t(0,1,0);
    eye = vec3f_t(0,0,1);
    center = vec3f_t(0,0,0);
}

mat4f_t lrCamera::lrLookAt(){
    vec3f_t z = (eye-center).normalize();
    vec3f_t x = (up^z).normalize();
    vec3f_t y = (z^x).normalize();
    mat4f_t view;
    for(int i=0;i<3;i++){
        view[0][i] = x[i];
        view[1][i] = y[i];
        view[2][i] = z[i];
        view[i][3] = -center[i];
    }
    return view;
}

vec3f_t lrCamera::lrFront(){
    //return eye - center;
    return center - eye;
}
