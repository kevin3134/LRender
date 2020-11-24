#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../include/image.h"

/* image creating/releasing */

image_t *image_create(int width, int height, int channels) {
    int buffer_size = width * height * channels;
    image_t *image;

    return image;
}

void image_release(image_t *image) {
    free(image->buffer);
    free(image);
}

image_t *image_load(const char *filename) {
    return NULL;
}

void image_save(image_t *image, const char *filename) {
}

/* image processing */
// static void swap_bytes(unsigned char *a, unsigned char *b) {
//     unsigned char t = *a;
//     *a = *b;
//     *b = t;
// }

// void image_flip_h(image_t *image) {
// }

// void image_flip_v(image_t *image) {
// }

// image_t *image_resize(image_t *source, int width, int height) {
//     return source;
// }