#include "../include/lrGraphic.h"
#include "../include/lrMath.h"
#include "../include/lrImage.h"
#include "../include/lrTexture.h"

#include <assert.h>

static vec4f_t get_buffer_value(framebuffer_t *buffer, int row, int col) {
    int index = row * buffer->width + col;
    return buffer->colorBuffer[index];
}

static unsigned char float_to_uchar(float value) {
    return (unsigned char)(value * 255);
}


framebuffer_t *lrCreateFramebuffer(int width, int height) {
    vec4f_t defaultColor = {0, 0, 0, 1};
    float defaultDepth = 10000;
    int num_elems = width * height;
    framebuffer_t *framebuffer;

    assert(width > 0 && height > 0);

    framebuffer = (framebuffer_t*)malloc(sizeof(framebuffer_t));
    framebuffer->width = width;
    framebuffer->height = height;
    framebuffer->colorBuffer = (vec4f_t*)malloc(sizeof(vec4f_t) * num_elems);
    framebuffer->depthBuffer = (float*)malloc(sizeof(float) * num_elems);

    lrClearColorFramebuffer(framebuffer, defaultColor);
    lrClearDepthFramebuffer(framebuffer, defaultDepth);
    return framebuffer;
}

void lrReleaseFramebuffer(framebuffer_t *framebuffer) {
    free(framebuffer->colorBuffer);
    free(framebuffer->depthBuffer);
    free(framebuffer);
}

void lrClearColorFramebuffer(framebuffer_t *framebuffer, vec4f_t color) {
    int num_elems = framebuffer->width * framebuffer->height;
    for (int i = 0; i < num_elems; i++) {
        framebuffer->colorBuffer[i] = color;
    }
}

void lrClearDepthFramebuffer(framebuffer_t *framebuffer, float depth) {
    int num_elems = framebuffer->width * framebuffer->height;
    for (int i = 0; i < num_elems; i++) {
        framebuffer->depthBuffer[i] = depth;
    }
}

void lrDrawPoint2D(framebuffer_t *framebuffer, vec2i_t v, vec4f_t color){
    //y * width + x
    int index = v[1] * framebuffer->width + v[0];
    framebuffer->colorBuffer[index] = color;
}


void lrDrawLine2D(framebuffer_t *framebuffer, vec2i_t v1, vec2i_t v2, vec4f_t color){
    int row0 = v1[0];
    int row1 = v2[0];
    int col0 = v1[1];
    int col1 = v2[1];

    int row_distance = abs(row0 - row1);
    int col_distance = abs(col0 - col1);
    if (row_distance == 0 && col_distance == 0) {
        lrDrawPoint2D(framebuffer, v1, color);
    } else if (row_distance > col_distance) {
        int row;
        if (row0 > row1) {
            lrSwap(&row0, &row1);
            lrSwap(&col0, &col1);
        }
        for (row = row0; row <= row1; row++) {
            float t = (float)(row - row0) / (float)row_distance;
            int col = lrLerp(col0, col1, t);
            lrDrawPoint2D(framebuffer, vec2i_t(row, col), color);
        }
    } else {
        int col;
        if (col0 > col1) {
            lrSwap(&col0, &col1);
            lrSwap(&row0, &row1);
        }
        for (col = col0; col <= col1; col++) {
            float t = (float)(col - col0) / (float)col_distance;
            int row = lrLerp(row0, row1, t);
            lrDrawPoint2D(framebuffer, vec2i_t(row, col), color);
        }
    }
}


void lrDrawTriangleLine2D(framebuffer_t *framebuffer, vec2i_t v1, vec2i_t v2, vec2i_t v3, vec4f_t color){
    lrDrawLine2D(framebuffer, v1, v2, color);
    lrDrawLine2D(framebuffer, v1, v3, color);
    lrDrawLine2D(framebuffer, v2, v3, color);
}


void lrDrawTriangle2D(framebuffer_t *framebuffer, vec2i_t v1, vec2i_t v2, vec2i_t v3, vec4f_t color){
    //lrBarycentric(vec3f_t A, vec3f_t B, vec3f_t C, vec3f_t P)

    vec2i_t bboxmin(framebuffer->width-1,  framebuffer->height-1); 
    vec2i_t bboxmax(0, 0); 
    vec2i_t clamp(framebuffer->width-1,  framebuffer->height-1); 

    bboxmin[0] = lrMax(0, lrMin(lrMin(v1[0],v2[0]),v3[0]));
    bboxmin[1] = lrMax(0, lrMin(lrMin(v1[1],v2[1]),v3[1]));

    bboxmax[0] = lrMin(framebuffer->width-1, lrMax(lrMax(v1[0],v2[0]),v3[0]));
    bboxmax[1] = lrMin(framebuffer->width-1, lrMax(lrMax(v1[1],v2[1]),v3[1]));

    vec2i_t P; 

    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) { 
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) { 
            vec3f_t bc_screen  = lrBarycentric(vec3f_t(v1[0],v1[1],0),vec3f_t(v2[0],v2[1],0),vec3f_t(v3[0],v3[1],0),vec3f_t(P[0],P[1],0)); 
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue; 


            lrDrawPoint2D(framebuffer, P, color); 
        } 
    } 
}

void lrDrawTriangle3D(framebuffer_t *framebuffer, vec3i_t *postion, vec4f_t color){
    //lrBarycentric(vec3f_t A, vec3f_t B, vec3f_t C, vec3f_t P)

    vec3i_t v1= *postion;
    vec3i_t v2 = *(postion+1);
    vec3i_t v3 = *(postion+2);

    int width = framebuffer->width;
    int height = framebuffer->height;

    vec2i_t bboxmin(framebuffer->width-1,  framebuffer->height-1); 
    vec2i_t bboxmax(0, 0); 
    vec2i_t clamp(framebuffer->width-1,  framebuffer->height-1); 


    bboxmin[0] = lrMax(0, lrMin(lrMin(v1[0],v2[0]),v3[0]));
    bboxmin[1] = lrMax(0, lrMin(lrMin(v1[1],v2[1]),v3[1]));

    bboxmax[0] = lrMin(framebuffer->width-1, lrMax(lrMax(v1[0],v2[0]),v3[0]));
    bboxmax[1] = lrMin(framebuffer->width-1, lrMax(lrMax(v1[1],v2[1]),v3[1]));

    vec3i_t P; 

    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) { 
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) { 
            vec3f_t bc_screen  = lrBarycentric(vec3f_t(v1[0],v1[1],v1[2]),vec3f_t(v2[0],v2[1],v2[2]),vec3f_t(v3[0],v3[1],v3[2]),vec3f_t(P[0],P[1],P[2])); 
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue; 

            P.z = v1[2]*bc_screen[0] + v2[2]*bc_screen[1] + v3[2]*bc_screen[2];
            if(framebuffer->depthBuffer[int(P.x+P.y*width)] < P.z){
                framebuffer->depthBuffer[int(P.x+P.y*width)] = P.z;
                lrDrawPoint2D(framebuffer, vec2i_t(P.x,P.y), color); 
            }
        } 
    } 
}

void lrDrawTriangle3DTexture(framebuffer_t *framebuffer, lrTexture *texture, vec3i_t *postion, vec2f_t *textureUV){
    vec3i_t v1= *postion;
    vec3i_t v2 = *(postion+1);
    vec3i_t v3 = *(postion+2);

    int width = framebuffer->width;
    int height = framebuffer->height;

    vec2i_t bboxmin(framebuffer->width-1,  framebuffer->height-1); 
    vec2i_t bboxmax(0, 0); 
    vec2i_t clamp(framebuffer->width-1,  framebuffer->height-1); 


    bboxmin[0] = lrMax(0, lrMin(lrMin(v1[0],v2[0]),v3[0]));
    bboxmin[1] = lrMax(0, lrMin(lrMin(v1[1],v2[1]),v3[1]));

    bboxmax[0] = lrMin(framebuffer->width-1, lrMax(lrMax(v1[0],v2[0]),v3[0]));
    bboxmax[1] = lrMin(framebuffer->width-1, lrMax(lrMax(v1[1],v2[1]),v3[1]));

    vec3i_t P; 

    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) { 
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) { 
            if(P.y<0||P.y>800||P.x<0||P.y>800) continue;

            vec3f_t bc_screen  = lrBarycentric(vec3f_t(v1[0],v1[1],v1[2]),vec3f_t(v2[0],v2[1],v2[2]),vec3f_t(v3[0],v3[1],v3[2]),vec3f_t(P[0],P[1],P[2])); 
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue; 


            P.z = v1[2]*bc_screen[0] + v2[2]*bc_screen[1] + v3[2]*bc_screen[2];

            vec2f_t uv = textureUV[0]*bc_screen[0] + textureUV[1]*bc_screen[1] + textureUV[2]*bc_screen[2];

            vec4f_t color = texture->lrGetTextureValue(uv);


            if(framebuffer->depthBuffer[int(P.x+P.y*width)] > P.z){
                framebuffer->depthBuffer[int(P.x+P.y*width)] = P.z;
                lrDrawPoint2D(framebuffer, vec2i_t(P.x,P.y), color); 
                //framebuffer->drawCount++;
            }
        } 
    } 
}

void lrDrawTriangleShader(framebuffer_t *framebuffer, vec3i_t *postion, lrShader *shader, lrStatus *status){
    vec3i_t v1= *postion;
    vec3i_t v2 = *(postion+1);
    vec3i_t v3 = *(postion+2);

    int width = framebuffer->width;
    int height = framebuffer->height;

    vec2i_t bboxmin(framebuffer->width-1,  framebuffer->height-1); 
    vec2i_t bboxmax(0, 0); 
    vec2i_t clamp(framebuffer->width-1,  framebuffer->height-1); 


    bboxmin[0] = lrMax(0, lrMin(lrMin(v1[0],v2[0]),v3[0]));
    bboxmin[1] = lrMax(0, lrMin(lrMin(v1[1],v2[1]),v3[1]));

    bboxmax[0] = lrMin(framebuffer->width-1, lrMax(lrMax(v1[0],v2[0]),v3[0]));
    bboxmax[1] = lrMin(framebuffer->width-1, lrMax(lrMax(v1[1],v2[1]),v3[1]));

    vec3i_t P; 

    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) { 
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) { 
            if(P.y<0||P.y>800||P.x<0||P.y>800) continue;

            vec3f_t bc_screen  = lrBarycentric(vec3f_t(v1[0],v1[1],v1[2]),vec3f_t(v2[0],v2[1],v2[2]),vec3f_t(v3[0],v3[1],v3[2]),vec3f_t(P[0],P[1],P[2])); 
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue; 


            P.z = v1[2]*bc_screen[0] + v2[2]*bc_screen[1] + v3[2]*bc_screen[2];

            //vec2f_t uv = textureUV[0]*bc_screen[0] + textureUV[1]*bc_screen[1] + textureUV[2]*bc_screen[2];

            //vec4f_t color = texture->lrGetTextureValue(uv);
            vec4f_t color = vec4f_t(0,0,0,1);

            bool discard = shader->fragment(bc_screen,color,status);

            if(framebuffer->depthBuffer[int(P.x+P.y*width)] > P.z){
                framebuffer->depthBuffer[int(P.x+P.y*width)] = P.z;
                lrDrawPoint2D(framebuffer, vec2i_t(P.x,P.y), color); 
            }
        } 
    } 
}


void lrDrawTriangle(framebuffer_t *framebuffer, vec3i_t *postion, lrShader *shader, lrStatus *status){
    vec3i_t v1= *postion;
    vec3i_t v2 = *(postion+1);
    vec3i_t v3 = *(postion+2);

    int width = framebuffer->width;
    int height = framebuffer->height;

    vec2i_t bboxmin(framebuffer->width-1,  framebuffer->height-1); 
    vec2i_t bboxmax(0, 0); 
    vec2i_t clamp(framebuffer->width-1,  framebuffer->height-1); 


    bboxmin[0] = lrMax(0, lrMin(lrMin(v1[0],v2[0]),v3[0]));
    bboxmin[1] = lrMax(0, lrMin(lrMin(v1[1],v2[1]),v3[1]));

    bboxmax[0] = lrMin(framebuffer->width-1, lrMax(lrMax(v1[0],v2[0]),v3[0]));
    bboxmax[1] = lrMin(framebuffer->width-1, lrMax(lrMax(v1[1],v2[1]),v3[1]));

    vec3i_t P; 

    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) { 
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) { 
            if(P.y<0||P.y>800||P.x<0||P.y>800) continue;

            vec3f_t bc_screen  = lrBarycentric(vec3f_t(v1[0],v1[1],v1[2]),vec3f_t(v2[0],v2[1],v2[2]),vec3f_t(v3[0],v3[1],v3[2]),vec3f_t(P[0],P[1],P[2])); 
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue; 


            P.z = v1[2]*bc_screen[0] + v2[2]*bc_screen[1] + v3[2]*bc_screen[2];

            //vec2f_t uv = textureUV[0]*bc_screen[0] + textureUV[1]*bc_screen[1] + textureUV[2]*bc_screen[2];

            vec4f_t color = vec4f_t();

            bool discard = shader->fragment(bc_screen,color,status);

            if(framebuffer->depthBuffer[int(P.x+P.y*width)] > P.z){
                framebuffer->depthBuffer[int(P.x+P.y*width)] = P.z;
                lrDrawPoint2D(framebuffer, vec2i_t(P.x,P.y), color); 
            }
        } 
    } 
}



void lrBlitBuffer(framebuffer_t *src, image_t *dst){
    int width = lrMin(src->width, dst->width);
    int height = lrMin(src->height, dst->height);
    int r, c;

    assert(width > 0 && height > 0);
    assert(dst->channels == 3 || dst->channels == 4);

    for (r = 0; r < height; r++) {
        for (c = 0; c < width; c++) {
            int flipped_r = src->height - 1 - r;
            vec4f_t src_value = get_buffer_value(src, flipped_r, c);
            unsigned char *dst_pixel = lrGetPixel(dst, r, c);
            dst_pixel[0] = float_to_uchar(src_value.z);  /* blue */
            dst_pixel[1] = float_to_uchar(src_value.y);  /* green */
            dst_pixel[2] = float_to_uchar(src_value.x);  /* red */
        }
    }
}


void lrBlitImage(image_t *src, image_t *dst) {
    int width = lrMin(src->width, dst->width);
    int height = lrMin(src->height, dst->height);
    int r, c;

    assert(width > 0 && height > 0);
    assert(src->channels >= 1 && src->channels <= 4);
    assert(dst->channels == 3 || dst->channels == 4);

    for (r = 0; r < height; r++) {
        for (c = 0; c < width; c++) {
            int flipped_r = src->height - 1 - r;
            unsigned char *src_pixel = lrGetPixel(src, flipped_r, c);
            unsigned char *dst_pixel = lrGetPixel(dst, r, c);
            if (src->channels == 3 || src->channels == 4) {
                dst_pixel[0] = src_pixel[2];  /* red */
                dst_pixel[1] = src_pixel[1];  /* green */
                dst_pixel[2] = src_pixel[0];  /* blue */
            } else {
                unsigned char gray = src_pixel[0];
                dst_pixel[0] = dst_pixel[1] = dst_pixel[2] = gray;
            }
        }
    }
}

void lrBlitImageBGR(image_t *src, image_t *dst) {
    int width = lrMin(src->width, dst->width);
    int height = lrMin(src->height, dst->height);
    int r, c;

    assert(width > 0 && height > 0);
    assert(src->channels >= 1 && src->channels <= 4);
    assert(dst->channels == 3 || dst->channels == 4);

    for (r = 0; r < height; r++) {
        for (c = 0; c < width; c++) {
            int flipped_r = src->height - 1 - r;
            unsigned char *src_pixel = lrGetPixel(src, flipped_r, c);
            unsigned char *dst_pixel = lrGetPixel(dst, r, c);
            if (src->channels == 3 || src->channels == 4) {
                dst_pixel[0] = src_pixel[0];  /* blue */
                dst_pixel[1] = src_pixel[1];  /* green */
                dst_pixel[2] = src_pixel[2];  /* red */
            } else {
                unsigned char gray = src_pixel[0];
                dst_pixel[0] = dst_pixel[1] = dst_pixel[2] = gray;
            }
        }
    }
}
