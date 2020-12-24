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
    std::cout << "texture channel:" << channels <<std::endl;
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

vec4f_t lrTexture::lrGetTextureValue(vec2f_t uv){
    return vec4f_t();
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

vec4f_t lrColorTexture::lrGetTextureValue(vec2f_t uv){
    return lrGetTextureColor(uv);
}



lrColorTexture::~lrColorTexture(){
    free(buffer);
}




lrNormTexture::lrNormTexture(image_t *image) : lrTexture(image) {
    //TODO: assert channel is 3 or 4
}


vec4f_t lrNormTexture::lrGetTextureNorm(vec2f_t uv){
    int index = lrGetTextureIndex(uv);
    float blue =buffer[index]/255.0f * 2.0f -1.0f;
    float green =buffer[index+1]/255.0f * 2.0f -1.0f;
    float red =buffer[index+2]/255.0f * 2.0f -1.0f;
    // float blue =buffer[index]/255.0f;
    // float green =buffer[index+1]/255.0f;
    // float red =buffer[index+2]/255.0f;
    float alpha = 0;

    if(channels==4){
        alpha = buffer[index+3]/255.0f;
    }
    return vec4f_t(red,green,blue,alpha);
    //return vec4f_t(blue,green,red,alpha);
}

vec4f_t lrNormTexture::lrGetTextureValue(vec2f_t uv){
    return lrGetTextureNorm(uv);
}



lrNormTexture::~lrNormTexture(){
    free(buffer);
}

lrSpecTexture::lrSpecTexture(image_t *image) : lrTexture(image) {
    //TODO: assert channel is 1
}


vec4f_t lrSpecTexture::lrGetTextureSpec(vec2f_t uv){
    int index = lrGetTextureIndex(uv);
    //std::cout << (int)buffer[index] << std::endl;
    //float result =buffer[index]/255.0f * 2.0f -1.0f;
    float result =buffer[index];
    return vec4f_t(result,0,0,0);
}

vec4f_t lrSpecTexture::lrGetTextureValue(vec2f_t uv){
    return lrGetTextureSpec(uv);
}

lrSpecTexture::~lrSpecTexture(){
    free(buffer);
}
