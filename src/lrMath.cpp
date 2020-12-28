#include "../include/lrMath.h"


vec3f_t lrBarycentric(vec3f_t A, vec3f_t B, vec3f_t C, vec3f_t P) {
    vec3f_t s[2];
    for (int i=2; i--; ) {
        s[i][0] = C[i]-A[i];
        s[i][1] = B[i]-A[i];
        s[i][2] = A[i]-P[i];
    }
    vec3f_t u = s[0]^s[1];
    if (std::abs(u[2])>1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
        return vec3f_t(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    return vec3f_t(-1,1,1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}


float mat3f_t::determinant(){
    float a = 0 + m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
    float b = 0 - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]);
    float c = 0 + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    return a + b + c;
}

mat3f_t mat3f_t::adjoint(){
    mat3f_t adjoint;
    adjoint[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]);
    adjoint[0][1] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]);
    adjoint[0][2] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]);
    adjoint[1][0] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]);
    adjoint[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]);
    adjoint[1][2] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]);
    adjoint[2][0] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]);
    adjoint[2][1] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]);
    adjoint[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]);
    return adjoint;
}

mat3f_t mat3f_t::inverse(){
    return (this->inverse_transpose()).transpose();
}

mat3f_t mat3f_t::transpose() {
    mat3f_t transpose;
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            transpose.m[i][j] = m[j][i];
        }
    }
    return transpose;
}

mat3f_t mat3f_t::inverse_transpose() {
    mat3f_t adjoint, inverse_transpose;
    float determinant, inv_determinant;
    int i, j;

    adjoint = this->adjoint();
    determinant = this->determinant();
    inv_determinant = 1 / determinant;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            inverse_transpose[i][j] = adjoint[i][j] * inv_determinant;
        }
    }
    return inverse_transpose;
}



float mat4f_t::minor(int r, int c) {
    mat3f_t cut_down;
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            int row = i < r ? i : i + 1;
            int col = j < c ? j : j + 1;
            cut_down[i][j] = m[row][col];
        }
    }
    return cut_down.determinant();
}

float mat4f_t::cofactor(int r, int c){
    float sign = (r + c) % 2 == 0 ? 1.0f : -1.0f;
    float minor = this->minor(r, c);
    return sign * minor;
}

mat4f_t mat4f_t::adjoint(){
    mat4f_t adjoint;
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            adjoint[i][j] = this->cofactor(i, j);
        }
    }
    return adjoint;
}

mat4f_t mat4f_t::inverse(){
    return (this->inverse_transpose()).transpose();
}

mat4f_t mat4f_t::transpose(){
    mat4f_t transpose;
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            transpose[i][j] = m[j][i];
        }
    }
    return transpose;
}

mat4f_t mat4f_t::inverse_transpose(){
    mat4f_t adjoint, inverse_transpose;
    float determinant, inv_determinant;
    int i, j;

    adjoint = this->adjoint();
    determinant = 0;
    for (i = 0; i < 4; i++) {
        determinant += m[0][i] * adjoint[0][i];
    }
    inv_determinant = 1 / determinant;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            inverse_transpose[i][j] = adjoint[i][j] * inv_determinant;
        }
    }
    return inverse_transpose;
}
