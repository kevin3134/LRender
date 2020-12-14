#include "../include/lrCamera.h"


lrCamera::lrCamera(){
    worldUp = vec3f_t(0,1,0);
    eye = vec3f_t(0,0,0);
    center = vec3f_t(0,0,-1);

    updateCameraVectors();
}

void lrCamera::cameraMove(CameraMovement direction){
    //由于使用worldUp作为cross基座表，在绕x轴旋转时会导致，right方向反转
    //因此，限制在上下方向上超过 180的旋转
    //另外，当前旋转对旋转后位置归一化
    
    vec3f_t prevEye = eye;
    if (direction == UPWARD){
        eye = eye + up * SPEED;
        if((prevEye.z+1)*(eye.z+1)<0){
            eye = prevEye;
        }
    }else if (direction == DOWNWARD){
        eye = eye - up * SPEED;
        if((prevEye.z+1)*(eye.z+1)<0){
            eye = prevEye;
        }
    }else if (direction == LEFT){
        eye = eye - right * SPEED;
    }else if (direction == RIGHT){
        eye = eye + right * SPEED;
    }

    eye = (eye - center).normalize() * distance + center;
        
    updateCameraVectors();
}

mat4f_t lrCamera::lrLookAt(){

    vec3f_t z = front;
    vec3f_t x = right;
    vec3f_t y = up;

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
    // front = (center - eye).normalize();
    // return center - eye;
    return front;
}


void lrCamera::updateCameraVectors(){
    front = (eye - center).normalize();
    right = (front^worldUp).normalize();
    up = (right^front).normalize();
}
