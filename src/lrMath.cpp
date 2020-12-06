#include "../include/lrMath.h"

vec3f_t mat3f_mul_vec3f(mat3f_t m, vec3f_t v) {

    std::cout<< m[0] << std::endl;

    float product[3];
    int i;
    for (i = 0; i < 3; i++) {
        float a = m.m[i][0] * v.x;
        float b = m.m[i][1] * v.y;
        float c = m.m[i][2] * v.z;
        product[i] = a + b + c;
        //std::cout<< m[0] << std::endl;
    }
    return vec3f_t(product[0], product[1], product[2]);
}
mat3f_t mat3f_mul_mat3f(mat3f_t a, mat3f_t b) {
    mat3f_t m(0.f);
    int i, j, k;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) {
                m.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return m;
}
mat3f_t mat3f_transpose(mat3f_t m) {
    mat3f_t transpose;
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            transpose.m[i][j] = m.m[j][i];
        }
    }
    return transpose;
}
mat3f_t mat3f_inverse(mat3f_t m) {
    return mat3f_transpose(mat3f_inverse_transpose(m));
}
static float mat3f_determinant(mat3f_t m) {
    float a = +m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]);
    float b = -m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]);
    float c = +m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);
    return a + b + c;
}
static mat3f_t mat3f_adjoint(mat3f_t m) {
    mat3f_t adjoint;
    adjoint.m[0][0] = +(m.m[1][1] * m.m[2][2] - m.m[2][1] * m.m[1][2]);
    adjoint.m[0][1] = -(m.m[1][0] * m.m[2][2] - m.m[2][0] * m.m[1][2]);
    adjoint.m[0][2] = +(m.m[1][0] * m.m[2][1] - m.m[2][0] * m.m[1][1]);
    adjoint.m[1][0] = -(m.m[0][1] * m.m[2][2] - m.m[2][1] * m.m[0][2]);
    adjoint.m[1][1] = +(m.m[0][0] * m.m[2][2] - m.m[2][0] * m.m[0][2]);
    adjoint.m[1][2] = -(m.m[0][0] * m.m[2][1] - m.m[2][0] * m.m[0][1]);
    adjoint.m[2][0] = +(m.m[0][1] * m.m[1][2] - m.m[1][1] * m.m[0][2]);
    adjoint.m[2][1] = -(m.m[0][0] * m.m[1][2] - m.m[1][0] * m.m[0][2]);
    adjoint.m[2][2] = +(m.m[0][0] * m.m[1][1] - m.m[1][0] * m.m[0][1]);
    return adjoint;
}
mat3f_t mat3f_inverse_transpose(mat3f_t m) {
    mat3f_t adjoint, inverse_transpose;
    float determinant, inv_determinant;
    int i, j;

    adjoint = mat3f_adjoint(m);
    determinant = mat3f_determinant(m);
    inv_determinant = 1 / determinant;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            inverse_transpose.m[i][j] = adjoint.m[i][j] * inv_determinant;
        }
    }
    return inverse_transpose;
}

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