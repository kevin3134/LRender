#ifndef LRTEXTURE_H
#define LRTEXTURE_H

#include "lrImage.h"
#include "lrMath.h"

class lrTexture{
    public:
        //lrTexture(const char *filename);
        lrTexture(image_t *image);
        ~lrTexture();

        virtual vec4f_t lrGetTextureValue(vec2f_t uv);


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
        virtual vec4f_t lrGetTextureValue(vec2f_t uv);

};


//channels should be 3 or 4
class lrNormTexture : public lrTexture{
    public:
        lrNormTexture(image_t *image);
        ~lrNormTexture();

        vec4f_t lrGetTextureNorm(vec2f_t uv);
        virtual vec4f_t lrGetTextureValue(vec2f_t uv);

};


#endif
