#ifndef LRMESH_H
#define LRMESH_H

#include <limits>
#include <vector>
#include "../include/lrMath.h"

class lrMesh{
    public:
        lrMesh(const char *filename);
        ~lrMesh();

        int countEBO();

        vec3f_t getVBOPostion(int n);        
        vec3f_t getScaledVBOPostion(int n);

        std::vector<vec3i_t> getEBO(int n);
        vec3i_t getEBOVetex(int n);
        vec3i_t getEBOTexture(int n);


        vec2f_t getTextureUV(int n);

    private:
        //list of vertices (x,y,z)
        std::vector<vec3f_t> VBOPosition;
        //list of surface (vertex/uv/normal for three vertices on the surface)
        std::vector<std::vector<vec3i_t>> EBO;
        //list of norm for surfaces
        std::vector<vec3f_t> VBONorm;
        //list of texture uv
        std::vector<vec2f_t> VBOTexture;

        float minXYZ[3] = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
        float maxXYZ[3] = {std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()};
        float maxScale = 0;

};

lrMesh lrLoadMesh(const char *filename);

#endif