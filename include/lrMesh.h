#ifndef LRMESH_H
#define LRMESH_H

#include <vector>
#include "../include/lrMath.h"

class lrMesh{
    public:
        lrMesh(const char *filename);
        ~lrMesh();

        int countEBO();

        vec3f_t getVBOPostion(int n);
        std::vector<vec3i_t> getEBO(int n);

        vec3i_t getEBOVetex(int n);


        

    private:
        //list of vertices (x,y,z)
        std::vector<vec3f_t> VBOPosition;
        //list of surface (vertex/uv/normal for three vertices on the surface)
        std::vector<std::vector<vec3i_t>> EBO;
        //list of norm for surfaces
        std::vector<vec3f_t> VBONorm;
        //list of texture uv
        std::vector<vec2f_t> VBOTexture;

};

lrMesh lrLoadMesh(const char *filename);

#endif