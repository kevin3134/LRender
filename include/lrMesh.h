#ifndef LRMESH_H
#define LRMESH_H

#include <vector>
#include "../include/lrMath.h"

class lrMesh{
    public:
        lrMesh(const char *filename);
        ~lrMesh();

    private:
        //list of vertices (x,y,z)
        std::vector<vec3f_t> VBOPosition;
        //list of surface (indice for three vertices on the surface)
        std::vector<std::vector<vec3i_t>> EBO;
        //list of norm for surfaces
        std::vector<vec3f_t> VBONorm;
        //list of texture uv
        std::vector<vec2f_t> VBOTexture;
};

lrMesh lrLoadMesh(const char *filename);

#endif