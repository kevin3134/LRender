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

int lrTexture::lrGetTextureIndex(vec2f_t uv){
    //row = which height = y; col = which width = x
    int row = (int)(uv.y*height);
    int col = (int)(uv.x*width);
    int index = row * width * channels + col * channels;

    return index;
}


lrColorTexture::lrColorTexture(image_t *image) : lrTexture(image) {
    //TODO: assert channel is 3 or 4
}


vec4f_t lrColorTexture::lrGetTextureColor(vec2f_t uv){
    int index = lrGetTextureIndex(uv);
    float blue =buffer[index]/255.0f;
    float green =buffer[index+1]/255.0f;
    float red =buffer[index+2]/255.0f;
    float alpha = 0;

    if(channels==4){
        alpha = buffer[index+3]/255.0f;
    }
    return vec4f_t(red,green,blue,alpha);
}



lrColorTexture::~lrColorTexture(){
    free(buffer);
}
