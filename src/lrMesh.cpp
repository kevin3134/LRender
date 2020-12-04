#include "../include/lrMesh.h"
#include "../include/lrMacro.h"

#include <assert.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

//load vertices info from obj file
lrMesh::lrMesh(const char *filename){
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;

    while(!in.eof()){
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            vec3f_t v;
            for (int i=0;i<3;i++) iss >> v[i];
            VBOPosition.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            vec3f_t n;
            for (int i=0;i<3;i++) iss >> n[i];
            VBONorm.push_back(n);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            vec2f_t uv;
            for (int i=0;i<2;i++) iss >> uv[i];
            VBOTexture.push_back(uv);
        }  else if (!line.compare(0, 2, "f ")) {
            std::vector<vec3i_t> f;
            vec3i_t tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                for (int i=0; i<3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
                f.push_back(tmp);
            }
            EBO.push_back(f);
        }
    }

    std::cerr << "# v# " << VBOPosition.size() << " f# "  << EBO.size() << " vt# " << VBOTexture.size() << " vn# " << VBONorm.size() << std::endl;
    in.close();
}

lrMesh::~lrMesh(){}



lrMesh lrLoadMesh(const char *filename){return lrMesh(filename);}

