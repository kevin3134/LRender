#ifndef LRCAMERA_H
#define LRCAMERA_H

#include "lrMath.h"

enum CameraMovement {
    UPWARD,
    DOWNWARD,
    LEFT,
    RIGHT,
    STOP
};

const float SPEED = 0.08f;


class lrCamera{
    public:
        lrCamera();
        ~lrCamera();

        void setCenter(vec3f_t c){
            center = c;
            updateCameraVectors();
        }

        void setEye(vec3f_t e){
            eye = e; 
            updateCameraVectors();
        }

        void cameraMove(CameraMovement direction);

        mat4f_t lrLookAt();
        vec3f_t lrFront();

        vec3f_t lrgetEye(){return eye;}
        vec3f_t lrgetRight(){return right;}
    private:
        vec3f_t eye;
        vec3f_t center;
        vec3f_t worldUp;

        vec3f_t up;
        vec3f_t front;
        vec3f_t right;

        void updateCameraVectors();
};


#endif