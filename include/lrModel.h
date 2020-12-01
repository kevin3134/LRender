#ifndef LRMODEL_H
#define LRMODEL_H

#include <vector>
#include "../include/lrMath.h"

class Model{
    public:
        Model(const char *filename);
        ~Model();
    private:
        //list of vertices (x,y,z)
        std::vector<vec3f_t> verts;
        //list of surface (indice for three vertices on the surface)
        std::vector<std::vector<vec3i_t>> faces;
        //list of norm for surfaces
        std::vector<vec3f_t> norms;
        //list of texture uv
        std::vector<vec2f_t> uv;
};


#endif