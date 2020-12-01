#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/lrImage.h"
#include "../include/lrMath.h"


/* image creating/releasing */
//TODO: 1, modified style 2, some functions into constructor
image_t *image_create(int width, int height, int channels) {
    int buffer_size = width * height * channels;
    image_t *image;

    assert(width > 0 && height > 0 && channels >= 1 && channels <= 4);

    image = (image_t*)malloc(sizeof(image_t));
    image->width = width;
    image->height = height;
    image->channels = channels;
    image->buffer = (unsigned char*)malloc(buffer_size);
    memset(image->buffer, 0, buffer_size);

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
unsigned char *get_pixel(image_t *image, int row, int col) {
    int index = row * image->width * image->channels + col * image->channels;
    return &(image->buffer[index]);
}

void image_flip_h(image_t *image) {
    int half_width = image->width / 2;
    int r, c, k;
    for (r = 0; r < image->height; r++) {
        for (c = 0; c < half_width; c++) {
            int flipped_c = image->width - 1 - c;
            unsigned char *pixel1 = get_pixel(image, r, c);
            unsigned char *pixel2 = get_pixel(image, r, flipped_c);
            for (k = 0; k < image->channels; k++) {
                lrSwap(&pixel1[k], &pixel2[k]);
            }
        }
    }
}

void image_flip_v(image_t *image) {
    int half_height = image->height / 2;
    int r, c, k;
    for (r = 0; r < half_height; r++) {
        for (c = 0; c < image->width; c++) {
            int flipped_r = image->height - 1 - r;
            unsigned char *pixel1 = get_pixel(image, r, c);
            unsigned char *pixel2 = get_pixel(image, flipped_r, c);
            for (k = 0; k < image->channels; k++) {
                lrSwap(&pixel1[k], &pixel2[k]);
            }
        }
    }
}

//tga
#define TGA_HEADER_SIZE 18

static unsigned char read_byte(FILE *file) {
    int byte = fgetc(file);
    assert(byte != EOF);
    return (unsigned char)byte;
}

static void read_bytes(FILE *file, void *buffer, int size) {
    int count = (int)fread(buffer, 1, size, file);
    assert(count == size);
    //UNUSED_VAR(count);
}

static void write_bytes(FILE *file, void *buffer, int size) {
    int count = (int)fwrite(buffer, 1, size, file);
    assert(count == size);
    //UNUSED_VAR(count);
}

static int get_buffer_size(image_t *image) {
    return image->width * image->height * image->channels;
}

static void load_tga_rle(FILE *file, image_t *image) {
    unsigned char *buffer = image->buffer;
    int channels = image->channels;
    int buffer_size = get_buffer_size(image);
    int elem_count = 0;
    while (elem_count < buffer_size) {
        unsigned char header = read_byte(file);
        int rle_packet = header & 0x80;
        int pixel_count = (header & 0x7F) + 1;
        unsigned char pixel[4];
        int i, j;
        assert(elem_count + pixel_count * channels <= buffer_size);
        if (rle_packet) {  /* rle packet */
            for (j = 0; j < channels; j++) {
                pixel[j] = read_byte(file);
            }
            for (i = 0; i < pixel_count; i++) {
                for (j = 0; j < channels; j++) {
                    buffer[elem_count++] = pixel[j];
                }
            }
        } else {           /* raw packet */
            for (i = 0; i < pixel_count; i++) {
                for (j = 0; j < channels; j++) {
                    buffer[elem_count++] = read_byte(file);
                }
            }
        }
    }
    // std::cout << elem_count << std::endl;
    // std::cout << buffer_size << std::endl;
    assert(elem_count == buffer_size);
}


image_t *lrLoadTGAImage(const char *filename){
    unsigned char header[TGA_HEADER_SIZE];
    int width, height, depth, channels;
    int idlength, imgtype, imgdesc;
    image_t *image;
    FILE *file;
    
    file = fopen(filename, "rb");
    assert(file != NULL);
    read_bytes(file, header, TGA_HEADER_SIZE);

    width = header[12] | (header[13] << 8);
    height = header[14] | (header[15] << 8);
    assert(width > 0 && height > 0);
    depth = header[16];
    assert(depth == 8 || depth == 24 || depth == 32);
    channels = depth / 8;

    // std::cout << "width: " << width << std::endl;
    // std::cout << "height: " << height << std::endl;
    // std::cout << "channels: " << channels << std::endl;



    image = image_create(width, height, channels);

    // std::cout << "image buffer size: " << get_buffer_size(image) << std::endl;



    idlength = header[0];
    assert(idlength == 0);
    //UNUSED_VAR(idlength);
    imgtype = header[2];
    if (imgtype == 2 || imgtype == 3) {           /* uncompressed */
        read_bytes(file, image->buffer, get_buffer_size(image));
    } else if (imgtype == 10 || imgtype == 11) {  /* run-length encoded */
        load_tga_rle(file, image);
    } else {
        assert(0);
    }
    fclose(file);

    imgdesc = header[17];
    if (imgdesc & 0x20) {
        image_flip_v(image);
    }
    if (imgdesc & 0x10) {
        image_flip_h(image);
    }
    return image;
}
