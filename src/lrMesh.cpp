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
            for (int i=0;i<3;i++){
                iss >> v[i];
                if(v[i] >maxXYZ[i]) maxXYZ[i] = v[i];
                if(v[i] <minXYZ[i]) minXYZ[i] = v[i];
            } 
            
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

    

    for(int i=0;i<3;i++){
        maxScale = lrMax((maxXYZ[i]-minXYZ[i]), maxScale);
    }
    std::cerr << "# v# " << VBOPosition.size() << " f# "  << EBO.size() << " vt# " << VBOTexture.size() << " vn# " << VBONorm.size() << std::endl;

    std::cerr <<"max: x:"<<maxXYZ[0]<<" y:"<<maxXYZ[1]<<" z:"<<maxXYZ[2]<<std::endl;
    std::cerr <<"min: x:"<<minXYZ[0]<<" y:"<<minXYZ[1]<<" z:"<<minXYZ[2] << std::endl;
    std::cerr <<"max scale: "<<maxScale << std::endl;
    in.close();
}

lrMesh::~lrMesh(){}




int lrMesh::countEBO(){return EBO.size();}



vec3f_t lrMesh::getVBOPostion(int n){
    return VBOPosition[n];
}

vec3f_t lrMesh::getScaledVBOPostion(int n){
    vec3f_t VBOPositionOrigin = VBOPosition[n];
    vec3f_t VBOPositionScaled = {
            (VBOPositionOrigin[0]-minXYZ[0])/maxScale, 
            (VBOPositionOrigin[1]-minXYZ[1])/maxScale, 
            (VBOPositionOrigin[2]-minXYZ[2])/maxScale};
    return VBOPositionScaled;
}


std::vector<vec3i_t> lrMesh::getEBO(int n){
    return EBO[n];
}


vec3i_t lrMesh::getEBOVetex(int n){
    std::vector<vec3i_t> currentEBO = getEBO(n);
    vec3i_t EBOVetex;
    for(int i=0;i<3;i++){
        EBOVetex[i] = currentEBO[i].x;
    }
    return EBOVetex;
}

vec3i_t lrMesh::getEBOTexture(int n){
    std::vector<vec3i_t> currentEBO = getEBO(n);
    vec3i_t EBOTexture;
    for(int i=0;i<3;i++){
        EBOTexture[i] = currentEBO[i].y;
    }
    return EBOTexture;
}

vec2f_t lrMesh::getTextureUV(int n){
    return VBOTexture[n];
}


lrMesh lrLoadMesh(const char *filename){return lrMesh(filename);}

