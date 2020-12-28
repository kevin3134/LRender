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
            // uniform_MIT = (status->projection * status->view * status->model).inverse_transpose();
        }
    protected:
        // mat4f_t uniform_M; //mvp
        // mat4f_t uniform_MIT; //(mvp).invert_transpose()
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
            vec4f_t textureColor = status->texture0->lrGetTextureValue(uv);

            vec4f_t n4 = (status->texture1->lrGetTextureValue(uv));
            n4.w = 0;
            vec3f_t n = vec3f_t(n4.x, n4.y, n4.z).normalize();

            vec3f_t l = (status->lightDir).normalize();
            vec3f_t r = (n*(n*l*2.f) - l).normalize(); 


            float diff = std::max(0.f, n*l);
            vec4f_t s4 = (status->texture2->lrGetTextureValue(uv));
            
            //用以下表示显示为角度高光
            // https://learnopengl.com/code_viewer.php?code=advanced-lighting/normal_mapping&type=fragment
            //front is already normalized
            // vec3f_t front = status->camera->lrFront();
            // vec3f_t halfway = (front+r).normalize();
            // float spec = pow(std::max(halfway*n, 0.0f), s4.x);

            //用以下显示为根据位置的高光 （来自tinyrender）
            // https://github.com/ssloy/tinyrenderer/wiki/Lesson-6-Shaders-for-the-software-renderer
            //float spec = pow(std::max(r.z, 0.0f), s4.x);

            // 用光照和视角夹角算，似乎效果最佳
            //https://github.com/jorgonz/OpenGL_Diffuse-Specular_Maps_Example/blob/master/Example_Scene_Light/shaders/fShader.fs
            vec3f_t front = status->camera->lrFront();
            float spec = pow(std::max(r*front, 0.0f), s4.x);

            for(int i=0;i<3;i++){
                //color[i] = lrMin( (float)(textureColor[i]*(0.8*diff+0.8*spec)+0.05) , 0.9999f);
                color[i] = diff;
            }
            return false;
        }
    private:
        vec3f_t norms[3];
        vec2f_t uvs[3];
};

class lrPhongTangentShader : public lrShader {
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

            ndc_tri[nthvert] = vec3f_t(gl_Vertex.x/gl_Vertex.w,gl_Vertex.y/gl_Vertex.w, gl_Vertex.z/gl_Vertex.w);

            return status->viewPort * status->projection * status->view * status->model * gl_Vertex;
        }

        virtual bool fragment(vec3f_t bar, vec4f_t &color, lrStatus *status){
            vec2f_t uv = uvs[0]*bar[0]+uvs[1]*bar[1]+uvs[2]*bar[2];
            vec4f_t textureColor = status->texture0->lrGetTextureValue(uv);

            vec4f_t n4 = (status->texture3->lrGetTextureValue(uv));


            vec3f_t tempNorm = norms[0]*bar[0]+norms[1]*bar[1]+norms[2]*bar[2];
            n4.w = 0;

            mat3f_t A;
            A[0] = ndc_tri[1] - ndc_tri[0];
            A[1] = ndc_tri[2] - ndc_tri[0];
            A[2] = tempNorm; 



            mat3f_t Ai = A.inverse();

            vec3f_t i = Ai * vec3f_t(uvs[1][0]-uvs[0][0],uvs[2][0]-uvs[0][0],0);
            vec3f_t j = Ai * vec3f_t(uvs[1][1]-uvs[0][1],uvs[2][1]-uvs[0][1],0);


            mat3f_t B;
            B[0]=i.normalize();
            B[1]=j.normalize();
            B[2]=tempNorm; 

            //TODO: check if this step is correct
            B = B.transpose();

            vec3f_t nn = B*vec3f_t(n4.x,n4.y,n4.z);

            vec3f_t n = nn.normalize();

            vec3f_t l = (status->camera->lrFront()).normalize();
            vec3f_t r = (n*(n*l*2.f) - l).normalize(); 


            float diff = std::max(0.f, n*l);
            vec4f_t s4 = (status->texture2->lrGetTextureValue(uv));
            

            vec3f_t front = status->camera->lrFront();
            float spec = pow(std::max(r*front, 0.0f), s4.x);

            for(int i=0;i<3;i++){
                color[i] = lrMin( (float)(textureColor[i]*(0.8*diff+0.8*spec)+0.05) , 0.9999f);
                //color[i] = textureColor[i];
                //color[i] = diff;
            }
            return false;
        }
    private:
        vec3f_t norms[3];
        vec2f_t uvs[3];
        mat3f_t ndc_tri;

        
};



#endif