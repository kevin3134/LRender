#include "../include/lrGraphic.h"
#include "../include/lrMath.h"
#include "../include/lrImage.h"

#include <assert.h>

static vec4f_t get_buffer_value(framebuffer_t *buffer, int row, int col) {
    int index = row * buffer->width + col;
    return buffer->colorbuffer[index];
}

unsigned char *private_get_pixel(image_t *image, int row, int col) {
    int index = row * image->width * image->channels + col * image->channels;
    return &(image->buffer[index]);
}

static unsigned char float_to_uchar(float value) {
    return (unsigned char)(value * 255);
}


framebuffer_t *framebuffer_create(int width, int height) {
    vec4f_t default_color = {0, 0, 0, 1};
    float default_depth = 1;
    int num_elems = width * height;
    framebuffer_t *framebuffer;

    assert(width > 0 && height > 0);

    framebuffer = (framebuffer_t*)malloc(sizeof(framebuffer_t));
    framebuffer->width = width;
    framebuffer->height = height;
    framebuffer->colorbuffer = (vec4f_t*)malloc(sizeof(vec4f_t) * num_elems);

    framebuffer_clear_color(framebuffer, default_color);

    return framebuffer;
}

void framebuffer_release(framebuffer_t *framebuffer) {
    free(framebuffer->colorbuffer);
    free(framebuffer);
}

void framebuffer_clear_color(framebuffer_t *framebuffer, vec4f_t color) {
    int num_elems = framebuffer->width * framebuffer->height;
    int i;
    for (i = 0; i < num_elems; i++) {
        framebuffer->colorbuffer[i] = color;
    }
}

void lrDrawPoint2D(framebuffer_t *framebuffer, vec2i_t v, vec4f_t color){
    int index = v[0] * framebuffer->width + v[1];
    framebuffer->colorbuffer[index] = color;
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


void lrDrawTriangle2D(framebuffer_t *framebuffer, vec2i_t v1, vec2i_t v2, vec2i_t v3, vec4f_t color){
    lrDrawLine2D(framebuffer, v1, v2, color);
    lrDrawLine2D(framebuffer, v1, v3, color);
    lrDrawLine2D(framebuffer, v2, v3, color);
}


void blit_buffer_rgb(framebuffer_t *src, image_t *dst){
    int width = lrMin(src->width, dst->width);
    int height = lrMin(src->height, dst->height);
    int r, c;

    assert(width > 0 && height > 0);
    assert(dst->channels == 3 || dst->channels == 4);

    for (r = 0; r < height; r++) {
        for (c = 0; c < width; c++) {
            int flipped_r = src->height - 1 - r;
            vec4f_t src_value = get_buffer_value(src, flipped_r, c);
            unsigned char *dst_pixel = private_get_pixel(dst, r, c);
            dst_pixel[0] = float_to_uchar(src_value.z);  /* blue */
            dst_pixel[1] = float_to_uchar(src_value.y);  /* green */
            dst_pixel[2] = float_to_uchar(src_value.x);  /* red */
        }
    }
}


void blit_image_rgb(image_t *src, image_t *dst) {
    int width = lrMin(src->width, dst->width);
    int height = lrMin(src->height, dst->height);
    int r, c;

    assert(width > 0 && height > 0);
    assert(src->channels >= 1 && src->channels <= 4);
    assert(dst->channels == 3 || dst->channels == 4);

    for (r = 0; r < height; r++) {
        for (c = 0; c < width; c++) {
            int flipped_r = src->height - 1 - r;
            unsigned char *src_pixel = private_get_pixel(src, flipped_r, c);
            unsigned char *dst_pixel = private_get_pixel(dst, r, c);
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
