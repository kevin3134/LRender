#include <iostream>
#include "../include/lrDisplay.h"
#include "../include/lrMath.h"
#include "../include/lrGraphic.h"
#include "../include/lrMesh.h"
#include "../include/lrTexture.h"
#include <assert.h>


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include <windows.h>
    #include "../src/lrDisplayWin.cpp"
#elif __APPLE__
    #include <unistd.h>
    #include "../src/displayMacTest.cpp"
#endif


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define lrSleep(a) Sleep(a)
#elif __APPLE__
    #define lrSleep(a) usleep(a)
#endif


void lrMathMatMulMat4(){
    vec4f_t vec1(1,1,1,1);
    vec4f_t vec2(1,1,1,1);


    mat4f_t mat1(1,2,3,4,0,1,4,6,5,6,0,7,0,0,0,1);
    mat4f_t mat2(1,2,3,4,0,1,4,6,5,6,0,7,0,0,0,1);

    mat4f_t mat3 = mat1*mat2;

    std::cout << mat1 <<std::endl;
    std::cout << mat2 <<std::endl;
    std::cout << mat3 <<std::endl;
}

void lrMathMatMulVec4(){
    vec4f_t vec1(1,1,1,1);
    vec4f_t vec2(1,5,8,12);

    mat4f_t mat1(1,2,3,4,0,1,4,6,5,6,0,7,0,0,0,1);
    mat4f_t mat2(1,2,3,4,0,1,4,6,5,6,0,7,0,0,0,1);

    vec4f_t vec3 = mat1*vec2;

    std::cout << mat1 <<std::endl;
    std::cout << vec2 <<std::endl;
    std::cout << vec3 <<std::endl;
}



int main(){

    lrMathMatMulVec4();

    return 0;
}