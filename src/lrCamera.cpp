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
    }else if(direction == FORWARD){
        subDistance();
    }else if(direction == BACKWARD){
        addDistance();
    }

    eye = (eye - center).normalize() * distance + center;
        
    updateCameraVectors();
}




mat4f_t lrCamera::lrLookAt(){

    vec3f_t f = front;
    vec3f_t s = right;
    vec3f_t u = up;

    mat4f_t Result;
    Result[0][0] = s.x;
    Result[0][1] = s.y;
    Result[0][2] = s.z;

    Result[1][0] = u.x;
    Result[1][1] = u.y;
    Result[1][2] = u.z;

    Result[2][0] = f.x;
    Result[2][1] = f.y;
    Result[2][2] = f.z;

    Result[0][3] =- (s*eye);
    Result[1][3] =- (u*eye);
    Result[2][3] =- (f*eye);
    return Result;
}

vec3f_t lrCamera::lrFront(){
    // front = (center - eye).normalize();
    // return center - eye;
    //return center -eye;
    return (eye - center).normalize();
    //return front;
}



// void lrCamera::updateCameraVectors(){
//     front = (center - eye).normalize();
//     right = (front^worldUp).normalize();
//     up = (right^front).normalize();
// }

void lrCamera::updateCameraVectors(){
    front = (eye - center).normalize();
    right = (worldUp^front).normalize();
    up = (front^right);
}

// void lrCamera::updateCameraVectors(){
//     front = (center - eye).normalize();
//     right = (worldUp^front).normalize();
//     up = (front^right).normalize();
// }

