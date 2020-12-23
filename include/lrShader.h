#ifndef LRSHADER_H
#define LRSHADER_H

#include "lrMath.h"
#include "lrStatus.h"

class lrShader {
    public:
        virtual ~lrShader(){};
        virtual vec4f_t vertex(int iface, int nthvert, lrStatus *status) = 0;
        virtual bool fragment(vec3f_t bar, vec4f_t &color, lrStatus *status) = 0;
        virtual void update(lrStatus *status){

            // uniform_M = status->projection * status->view * status->model;
            // uniform_MIT = uniform_M.inverse_transpose();

            uniform_M = status->projection * status->view * status->model;
            uniform_MIT = (status->projection * status->view * status->model).inverse_transpose();
            //std::cout<< uniform_MIT << std::endl;
        }
    protected:
        mat4f_t uniform_M; //mvp
        mat4f_t uniform_MIT; //(mvp).invert_transpose()
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
        int count = 0;
        virtual vec4f_t vertex(int EBO, int nthvert, lrStatus *status){
            vec4f_t gl_Vertex = vec4f_t(status->mesh->getVBOPostion(status->mesh->getEBOVetex(EBO)[nthvert]),1);

            vec3f_t norm = status->mesh->getNorm(status->mesh->getEBONorm(EBO)[nthvert]);
            norms[nthvert] = norm;

            vec2f_t uv = status->mesh->getTextureUV(status->mesh->getEBOTexture(EBO)[nthvert]);
            uvs[nthvert] = uv;

            //auto scale
            gl_Vertex = gl_Vertex * (2.0f/status->mesh->getScale());
            gl_Vertex.w = 1.0f;

            return status->viewPort * status->projection * status->view * status->model * gl_Vertex;
        }

        virtual bool fragment(vec3f_t bar, vec4f_t &color, lrStatus *status){
            vec2f_t uv = uvs[0]*bar[0]+uvs[1]*bar[1]+uvs[2]*bar[2];
            vec4f_t color0 = status->texture0->lrGetTextureValue(uv);

            //vec4f_t nTemp = status->texture1->lrGetTextureValue(uv);
            vec4f_t temp = (status->texture1->lrGetTextureValue(uv));
            temp.w = 0;
            temp = temp.normalize();

            
            vec3f_t n = vec3f_t(temp.x, temp.y, temp.z).normalize();
        
            
            vec3f_t front = (status->camera)->lrFront();

            // vec4f_t l4 = uniform_M * vec4f_t(front.x,front.y,front.z,1);
            // vec3f_t l = vec3f_t(l4.x, l4.y, l4.z).normalize();

            //vec3f_t l = front;
            vec3f_t l = vec3f_t(1,0,0);

            // vec3f_t r = (n*(n*l*2.f) - l).normalize(); 

            // float diff = std::max(0.f, n*l);

            float intensity = n*l;
            
            intensity = lrMax(0.0f,intensity);

            //std::cout<< intensity << std::endl;

            for(int i=0;i<3;i++){
                //color[i] = color0[i]*0.8*diff+0.05;
                color[i] = intensity;
            }

            return false;
        }
    private:
        vec3f_t norms[3];
        vec2f_t uvs[3];
};

#endif