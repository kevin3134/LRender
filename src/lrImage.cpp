#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/lrImage.h"
#include "../include/lrMath.h"


void image_t::init(int w, int h, int c){
    int buffer_size = w * h * c;

    assert(w > 0 && h > 0 && c >= 1 && c <= 4);

    width = w;
    height = h;
    channels = c;
    buffer = (unsigned char*)malloc(buffer_size);
    memset(buffer, 0, buffer_size);
}

image_t::image_t(const char *filename){

}

image_t::~image_t(){
    free(buffer);
}

//TODO: 1, modified style 2, some functions into constructor




/* image processing */
unsigned char *lrGetPixel(image_t *image, int row, int col) {
    int index = row * image->width * image->channels + col * image->channels;
    return &(image->buffer[index]);
}

void lrFlipImageHorzon(image_t *image) {
    int half_width = image->width / 2;
    int r, c, k;
    for (r = 0; r < image->height; r++) {
        for (c = 0; c < half_width; c++) {
            int flipped_c = image->width - 1 - c;
            unsigned char *pixel1 = lrGetPixel(image, r, c);
            unsigned char *pixel2 = lrGetPixel(image, r, flipped_c);
            for (k = 0; k < image->channels; k++) {
                lrSwap(&pixel1[k], &pixel2[k]);
            }
        }
    }
}

void lrFlipImageVertical(image_t *image) {
    int half_height = image->height / 2;
    int r, c, k;
    for (r = 0; r < half_height; r++) {
        for (c = 0; c < image->width; c++) {
            int flipped_r = image->height - 1 - r;
            unsigned char *pixel1 = lrGetPixel(image, r, c);
            unsigned char *pixel2 = lrGetPixel(image, flipped_r, c);
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
    //if input is set to be none, skip check file valid
    if(strcmp(filename, "none")==0){
        return nullptr;
    }

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

    image = new image_t(width, height, channels);


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
        lrFlipImageVertical(image);
    }
    if (imgdesc & 0x10) {
        lrFlipImageHorzon(image);
    }
    return image;
}
