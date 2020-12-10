#ifndef LRTEXTURE_H
#define LRTEXTURE_H

#include "lrImage.h"
#include "lrMath.h"

class lrTexture{
    public:
        //lrTexture(const char *filename);
        lrTexture(image_t *image);
        ~lrTexture();


    protected:
        int width;
        int height;
        int channels;
        unsigned char *buffer;

        int lrGetTextureIndex(vec2f_t uv);
};


//channels should be 3 or 4
class lrColorTexture : public lrTexture{
    public:
        lrColorTexture(image_t *image);
        ~lrColorTexture();

        vec4f_t lrGetTextureColor(vec2f_t uv);

};


#endif
