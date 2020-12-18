#ifndef LRSHADER_H
#define LRSHADER_H

#include "lrMath.h"

class lrShader {
    public:
        virtual ~lrShader(){};
        virtual vec4f_t vertex(int iface, int nthvert) = 0;
        virtual bool fragment(vec3f_t bar, vec4f_t &color) = 0;
};

#endif