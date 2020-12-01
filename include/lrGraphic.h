#ifndef LRGRAPHIC_H
#define LRGRAPHIC_H

#include "lrImage.h"
#include "lrMath.h"


typedef struct {
    int width, height;
    vec4f_t *colorbuffer;
    //float *depthbuffer;
} framebuffer_t;


framebuffer_t *framebuffer_create(int width, int height);
void framebuffer_release(framebuffer_t *framebuffer);
void framebuffer_clear_color(framebuffer_t *framebuffer, vec4f_t color);
// void framebuffer_clear_depth(framebuffer_t *framebuffer, float depth);


void blit_buffer_rgb(framebuffer_t *source, image_t *target);


void lrDrawPoint2D(framebuffer_t *framebuffer, vec2i_t v, vec4f_t color);
void lrDrawLine2D(framebuffer_t *framebuffer, vec2i_t v1, vec2i_t v2, vec4f_t color);


#endif