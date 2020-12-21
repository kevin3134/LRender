#ifndef LRSHADER_H
#define LRSHADER_H

#include "lrMath.h"
#include "lrStatus.h"

class lrShader {
    public:
        virtual ~lrShader(){};
        virtual vec4f_t vertex(int iface, int nthvert, lrStatus *status) = 0;
        virtual bool fragment(vec3f_t bar, vec4f_t &color, lrStatus *status) = 0;
};

class lrGouraudShader : public lrShader {
    public:
        virtual ~lrGouraudShader(){};
        virtual vec4f_t vertex(int EBO, int nthvert, lrStatus *status){
            vec3f_t norm = status->mesh->getNorm(status->mesh->getEBONorm(EBO)[nthvert]);
            vertexIntensity[nthvert] = lrMax(0.f, norm*status->camera->lrFront() );
            vec4f_t gl_Vertex = vec4f_t(status->mesh->getVBOPostion(status->mesh->getEBOVetex(EBO)[nthvert]),1);
            return status->viewPort * status->projection * status->view * status->model * gl_Vertex;
        }

        virtual bool fragment(vec3f_t bar, vec4f_t &color, lrStatus *status){
            float intensity = vertexIntensity*bar;
            color = vec4f_t(intensity,intensity,intensity,1);
            return false;
        }
    private:
        vec3f_t vertexIntensity;
};

#endif