#ifndef LRTEXTURE_H
#define LRTEXTURE_H

#include "lrImage.h"
#include "lrMath.h"

class lrTexture{
    public:
        //lrTexture(const char *filename);
        lrTexture(image_t *image);
        ~lrTexture();

        vec3i_t lrGetTextureRGB(int row, int col);
        vec4i_t lrGetTextureRGBA(int row, int col);

        vec3f_t lrGetTextureRGBbyUV(float u, float v);
        vec4f_t lrGetTextureRGBAbyUV(float u, float v);


    private:
        int width;
        int height;
        int channels = 4;
        unsigned char *buffer;

        int lrGetTextureIndex(int row, int col);

};


#endif
