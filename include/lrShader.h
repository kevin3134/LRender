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

//generate image in black and white; discribe theta between norm and front
class lrGouraudShader : public lrShader {
    public:
        virtual ~lrGouraudShader(){};
        virtual vec4f_t vertex(int EBO, int nthvert, lrStatus *status){
            vec3f_t norm = status->mesh->getNorm(status->mesh->getEBONorm(EBO)[nthvert]);
            vertexIntensity[nthvert] = lrMax(0.f, norm*status->camera->lrFront() );
            vec4f_t gl_Vertex = vec4f_t(status->mesh->getVBOPostion(status->mesh->getEBOVetex(EBO)[nthvert]),1);
            gl_Vertex = gl_Vertex * (2.0f/status->mesh->getScale());
            gl_Vertex.w = 1;
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

//generate image with texture
class lrGeneralShader : public lrShader {
    public:
        virtual ~lrGeneralShader(){};
        virtual vec4f_t vertex(int EBO, int nthvert, lrStatus *status){
            vec4f_t gl_Vertex = vec4f_t(status->mesh->getVBOPostion(status->mesh->getEBOVetex(EBO)[nthvert]),1);

            //auto scale
            gl_Vertex = gl_Vertex * (2.0f/status->mesh->getScale());
            gl_Vertex.w = 1.0f;

            return status->viewPort * status->projection * status->view * status->model * gl_Vertex;
        }

        virtual bool fragment(vec3f_t bar, vec4f_t &color, lrStatus *status){
            return false;
        }
};


//generate image with texture
class lrPhongShader : public lrShader {
    public:
        virtual ~lrPhongShader(){};
        virtual vec4f_t vertex(int EBO, int nthvert, lrStatus *status){
            vec4f_t gl_Vertex = vec4f_t(status->mesh->getVBOPostion(status->mesh->getEBOVetex(EBO)[nthvert]),1);

            //auto scale
            gl_Vertex = gl_Vertex * (2.0f/status->mesh->getScale());
            gl_Vertex.w = 1.0f;

            return status->viewPort * status->projection * status->view * status->model * gl_Vertex;
        }

        virtual bool fragment(vec3f_t bar, vec4f_t &color, lrStatus *status){
            return false;
        }
    private:
        mat4f_t uniform_M; //mvp
        mat4f_t uniform_MIT; //(mvp).invert_transpose()
};

#endif