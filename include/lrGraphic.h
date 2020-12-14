#ifndef LRGRAPHIC_H
#define LRGRAPHIC_H

#include "lrImage.h"
#include "lrMath.h"
#include "lrTexture.h"


typedef struct {
    int width, height;
    vec4f_t *colorBuffer;
    float *depthBuffer;
} framebuffer_t;


framebuffer_t *lrCreateFramebuffer(int width, int height);
void lrReleaseFramebuffer(framebuffer_t *framebuffer);
void lrClearColorFramebuffer(framebuffer_t *framebuffer, vec4f_t color);
void lrClearDepthFramebuffer(framebuffer_t *framebuffer, float depth);

void framebuffer_clear_depth(framebuffer_t *framebuffer, float depth);

//copy image data from source to target
void lrBlitBuffer(framebuffer_t *source, image_t *target);
void lrBlitImage(image_t *source, image_t *target);

void lrBlitImageBGR(image_t *source, image_t *target);


void lrDrawPoint2D(framebuffer_t *framebuffer, vec2i_t v, vec4f_t color);
void lrDrawLine2D(framebuffer_t *framebuffer, vec2i_t v1, vec2i_t v2, vec4f_t color);
void lrDrawTriangleLine2D(framebuffer_t *framebuffer, vec2i_t v1, vec2i_t v2, vec2i_t v3, vec4f_t color);



void lrDrawTriangle2D(framebuffer_t *framebuffer, vec2i_t v1, vec2i_t v2, vec2i_t v3, vec4f_t color);
void lrDrawTriangle3D(framebuffer_t *framebuffer, vec3i_t *postion, vec4f_t color);

void lrDrawTriangle3DTexture(framebuffer_t *framebuffer, lrColorTexture *texture, vec3i_t *postion, vec2f_t *textureUV);


static mat4f_t lrViewPort(int x, int y, int w, int h){
    float depth = 2000.f;

    mat4f_t m;
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = depth/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = depth/2.f;
    return m;
}


static mat4f_t lrPerspective(float fovy, float aspect, float near, float far){
    mat4f_t m;


    float z_range = far - near;
    float tanHalfFovy = tan(fovy/2.0f);

    m[0][0] = 1.0f/ (aspect * tanHalfFovy);
    m[1][1] = 1.0f/ tanHalfFovy;
    //m[2][2] = -(near + far) / (far - near);
    //m[3][2] = -1;
    //m[2][3] = -far * near /  (far - near);
    // m[2][3] = -1;
    // m[3][2] = -2 * far * near /  (far - near);
    //m[3][3] = 0;

    //std::cout << m <<std::endl;

    return m;
}


#endif