#include "../include/lrTexture.h"

#include <stdio.h>
#include <stdlib.h>

// lrTexture::lrTexture(const char *filename){
// }
        
lrTexture::lrTexture(image_t *image){
    width = image->width;
    height = image->height;
    channels = image->channels;
    int buffer_size = width * height * channels;

    buffer = (unsigned char*)malloc(buffer_size);

    for(int i=0;i<buffer_size;i++){
        buffer[i] = image->buffer[i];
    }
    std::cout << channels <<std::endl;
}

lrTexture::~lrTexture(){
    free(buffer);
}

int lrTexture::lrGetTextureIndex(int row, int col){
    int index = row * width * channels + col * channels;
    //std::cout << row << std::endl;
    return index;
}


vec3i_t lrTexture::lrGetTextureRGB(int row, int col){
    int index = lrGetTextureIndex(row,col);
    return vec3i_t((int)buffer[index], (int)buffer[index+1],(int)buffer[index+2]);
}

vec4i_t lrTexture::lrGetTextureRGBA(int row, int col){
    int index = lrGetTextureIndex(row,col);
    return vec4i_t((int)buffer[index], (int)buffer[index+1],(int)buffer[index+2],(int)buffer[index+3]);
}


vec3f_t lrTexture::lrGetTextureRGBbyUV(float u, float v){
    vec3i_t textureInt = lrGetTextureRGB((int)(u*width), (int)(v*height));
    std::cout << textureInt << std::endl;
    return vec3f_t(textureInt.x/255.0,textureInt.y/255.0,textureInt.z/255.0);
}


vec4f_t lrTexture::lrGetTextureRGBAbyUV(float u, float v){
    vec4i_t textureInt = lrGetTextureRGBA((int)(u*width), (int)(v*height));
    std::cout << textureInt << std::endl;
    return vec4f_t(textureInt.x/255.0,textureInt.y/255.0,textureInt.z/255.0,textureInt.w/255.0);
}
