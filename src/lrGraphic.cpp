#include "../include/lrGraphic.h"
#include "../include/lrMath.h"

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
    vec4f_t default_color = {0, .5, .5, 1};
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
