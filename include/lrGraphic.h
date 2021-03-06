#ifndef LRGRAPHIC_H
#define LRGRAPHIC_H

#include "lrImage.h"
#include "lrMath.h"
#include "lrTexture.h"
#include "lrShader.h"


typedef struct {
    int width, height;
    vec4f_t *colorBuffer;
    float *depthBuffer;
    int drawCount;
} framebuffer_t;


framebuffer_t *lrCreateFramebuffer(int width, int height);
void lrReleaseFramebuffer(framebuffer_t *framebuffer);

void lrClearColorFramebuffer(framebuffer_t *framebuffer, vec4f_t color);
void lrClearDepthFramebuffer(framebuffer_t *framebuffer, float depth);
void lrClearDefault(framebuffer_t *framebuffer);

//copy image data from source to target
void lrBlitBuffer(framebuffer_t *source, image_t *target);
void lrBlitImageRGB(image_t *source, image_t *target);

void lrBlitImageBGR(image_t *source, image_t *target);


void lrDrawPoint2D(framebuffer_t *framebuffer, vec2i_t v, vec4f_t color);
void lrDrawLine2D(framebuffer_t *framebuffer, vec2i_t v1, vec2i_t v2, vec4f_t color);
void lrDrawTriangleLine2D(framebuffer_t *framebuffer, vec2i_t v1, vec2i_t v2, vec2i_t v3, vec4f_t color);


//direct draw triangle to screen with 3 vertex in screen
void lrDrawTriangle2D(framebuffer_t *framebuffer, vec2i_t v1, vec2i_t v2, vec2i_t v3, vec4f_t color);

void lrDrawTriangle(framebuffer_t *framebuffer, vec3i_t *postion, lrShader *shader, lrStatus *status);


static mat4f_t lrViewPort(int x, int y, int w, int h){
    float depth = 100000.f;

    mat4f_t m;
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = depth/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = depth/2.f;

    //std::cout << m << std::endl;
    return m;
}


static mat4f_t lrPerspective(float fovy, float aspect, float near, float far){
    mat4f_t m;


    float z_range = far - near;
    float tanHalfFovy = tan(fovy/2.0f);

    m[0][0] = 1.0f/ (aspect * tanHalfFovy);
    m[1][1] = 1.0f/ tanHalfFovy;
    m[2][2] = -(near + far) / (far - near);
    m[2][3] = -2* far * near / (far - near);
    m[3][2] = -1;
    m[3][3] = 0;

    return m;
}

static bool lrShouldClip(vec4f_t vetex){
    if(abs(vetex.x)>abs(vetex.w)||
       abs(vetex.y)>abs(vetex.w)||
       abs(vetex.z)>abs(vetex.w)){

        return true;
    }
    return false;
} 


#endif