#ifndef LRMATH_H
#define LRMATH_H

#include <cmath>
#include <iostream>

//vec2_t related
template <class t> struct vec2_t {
    t x;
    t y;
    vec2_t<t>(){ x = y = t(); }
    vec2_t<t>(t _x, t _y){ x=_x; y=_y; }
    vec2_t<t>(const vec2_t<t> &V){ x = V.x; y = V.y; }

    vec2_t<t> operator +(const vec2_t<t> V) const { return vec2_t<t>(x+V.x, y+V.y); }
    vec2_t<t> operator -(const vec2_t<t> V) const { return vec2_t<t>(x-V.x, y-V.y); }
    vec2_t<t> operator *(float f)          const { return vec2_t<t>(x*f, y*f); }

    t& operator[](const int i) {
        if(i==0){return x;}
        else if(i==1){return y;}
        else{throw "incorrect index input in vec2!";}
    }
    template <class> friend std::ostream& operator<<(std::ostream& out, vec2_t<t>& V);

};

template <class t> std::ostream& operator<<(std::ostream& out, vec2_t<t>& V){
    out << "vec2: [" << V.x << ", "<<V.y << "]";
    return out;
}

typedef vec2_t<float> vec2f_t;
typedef vec2_t<int>   vec2i_t;


//vec3_t related
template <class t> struct vec3_t {
    t x;
    t y;
    t z;
    vec3_t<t>(){ x = y = z = t(); }
    vec3_t<t>(t _x, t _y, t _z){ x=_x; y=_y; z=_z;}
    vec3_t<t>(const vec3_t<t> &V){ x = V.x; y = V.y; z = V.z;}
    vec3_t<t>(const vec2_t<t> &V, t _z){ x = V.x; y = V.y; z = _z;}

    vec3_t<t> operator +(const vec3_t<t> V) const { return vec3_t<t>(x+V.x, y+V.y, z+V.z); }
    vec3_t<t> operator -(const vec3_t<t> V) const { return vec3_t<t>(x-V.x, y-V.y, z-V.z); }
    vec3_t<t> operator *(float f)          const { return vec3_t<t>(x*f, y*f, z*f); }
    vec3_t<t> operator ^(const vec3_t<t> &v) const { return vec3_t<t>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
    t operator *(vec3_t<t> V)          const { return x*V.x+y*V.y+z*V.z; }

    t& operator[](const int i) {
        if(i==0){return x;}
        else if(i==1){return y;}
        else if(i==2){return z;}
        else{throw "incorrect index input in vec3!";}
    }

    float norm () const { return std::sqrt(x*x+y*y+z*z); }
	vec3_t<t> & normalize(t l=1) { *this = (*this)*(l/norm()); return *this; }

    bool operator==(vec3_t<t> V){ return x==V.x&&y==V.y&&z==V.z; }
    bool operator!=(vec3_t<t> V){ return x!=V.x||y!=V.y||z!=V.z; }



    template <class> friend std::ostream& operator<<(std::ostream& out, vec3_t<t>& V);

};

template <class t> std::ostream& operator<<(std::ostream& out, vec3_t<t>& V){
    out << "vec3: [" << V.x << ", "<<V.y << ", "<<V.z << "]";
    return out;
}

typedef vec3_t<float> vec3f_t;
typedef vec3_t<int>   vec3i_t;


//vec4_t related
template <class t> struct vec4_t {
    t x;
    t y;
    t z;
    t w;
    void init(t _x, t _y, t _z, t _w){ x = _x; y = _y; z = _z; w=_w;}
    vec4_t<t>(){ init(t(),t(),t(),t()); }
    vec4_t<t>(t _x, t _y, t _z, t _w){ init(_x,_y,_z,_w);}
    vec4_t<t>(const vec4_t<t> &V){init(V.x,V.y,V.z,V.w);}
    vec4_t<t>(const vec3_t<t> &V, t _w){ init(V.x,V.y,V.z,_w);}

    vec4_t<t> operator +(const vec4_t<t> V) const { return vec4_t<t>(x+V.x, y+V.y, z+V.z, w+V.w); }
    vec4_t<t> operator -(const vec4_t<t> V) const { return vec4_t<t>(x-V.x, y-V.y, z-V.z, w-V.w); }
    vec4_t<t> operator *(float f)           const { return vec4_t<t>(x*f, y*f, z*f, w*f); }
    t operator *(vec4_t<t> V)               const { return x*V.x + y*V.y + z*V.z + w*V.w; }


    //TODO: need modification for float equal
    bool operator==(vec4_t<t> V){ return x==V.x&&y==V.y&&z==V.z&&w==V.w; }
    bool operator!=(vec4_t<t> V){ return x!=V.x||y!=V.y||z!=V.z||w!=V.w; }

    t& operator[](const int i) {
        if(i==0){return x;}
        else if(i==1){return y;}
        else if(i==2){return z;}
        else if(i==3){return w;}
        else{throw "incorrect index input in vec4!";}
    }
    template <class> friend std::ostream& operator<<(std::ostream& out, vec4_t<t>& V);
    float norm () const { return std::sqrt(x*x+y*y+z*z+w*w); }
    vec4_t<t> & normalize(t l=1) { *this = (*this)*(l/norm()); return *this; }

};

template <class t> std::ostream& operator<<(std::ostream& out, vec4_t<t>& V){
    out << "vec4: [" << V.x << ", "<<V.y << ", "<<V.z << ", "<<V.w << "]";
    return out;
}

typedef vec4_t<float> vec4f_t;
typedef vec4_t<int>   vec4i_t;



//mat related

struct mat3f_t{
    vec3f_t m[3];
    void init(vec3f_t row0, vec3f_t row1, vec3f_t row2){
        m[0]=row0;
        m[1]=row1;
        m[2]=row2;
    }

    mat3f_t(float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3){
        init( vec3f_t(x1,y1,z1) , vec3f_t(x2,y2,z2) , vec3f_t(x3,y3,z3) );
    }
    mat3f_t(){
        init(vec3f_t(1,0,0),vec3f_t(0,1,0),vec3f_t(0,0,1));
    }
    mat3f_t(vec3f_t row0, vec3f_t row1, vec3f_t row2){
        init(row0,row1,row2);
    }

    mat3f_t operator *(float f)     const { return mat3f_t(m[0]*f,m[1]*f,m[2]*f); }
    vec3f_t operator *(vec3f_t V)   const { return vec3f_t(m[0]*V,m[1]*V,m[2]*V); }
    mat3f_t operator *(mat3f_t V)   const {
        mat3f_t retMat;
        for(int row=0;row<3;row++){
            for(int col=0;col<3;col++){
                retMat[row][col] = m[row]*vec3f_t(V.m[0][col], V.m[1][col], V.m[2][col]);
            }
        }
        return retMat; 
    }
    vec3f_t& operator[](const int i) {
        if(i<=2){return m[i];}
        else{throw "incorrect index input in mat3!";}
    }
    bool operator== (mat3f_t M)  const {
        for(int i=0;i<3;i++){
            if(M[i]!=m[i]){
                return false;
            } 
        }
        return true;
    }

    bool operator!= (mat3f_t M)  const {
        for(int i=0;i<3;i++){
            if(M[i]==m[i]){
                return false;
            } 
        }
        return true;
    }


    friend std::ostream& operator<<(std::ostream& out, mat3f_t& mat){
        out << "|" << mat.m[0][0] << ", "<<mat.m[0][1] << ", "<<mat.m[0][2] << "|\n";
        out << "|" << mat.m[1][0] << ", "<<mat.m[1][1] << ", "<<mat.m[1][2] << "|\n";
        out << "|" << mat.m[2][0] << ", "<<mat.m[2][1] << ", "<<mat.m[2][2] << "|\n";
        return out;
    }


    float determinant();
    mat3f_t adjoint();
    mat3f_t inverse();
    mat3f_t transpose();
    mat3f_t inverse_transpose();
};

struct mat4f_t{
    //4 rows
    vec4f_t m[4];

    void init(vec4f_t row0, vec4f_t row1, vec4f_t row2, vec4f_t row3){
        m[0]=row0;
        m[1]=row1;
        m[2]=row2;
        m[3]=row3;
    }

    mat4f_t(float x1, float y1, float z1, float w1,
        float x2, float y2, float z2, float w2,
        float x3, float y3, float z3, float w3,
        float x4, float y4, float z4, float w4){
        init(vec4f_t(x1,y1,z1,w1),vec4f_t(x2,y2,z2,w2),vec4f_t(x3,y3,z3,w3),vec4f_t(x4,y4,z4,w4));
        }
    mat4f_t(){
        init(vec4f_t(1,0,0,0),vec4f_t(0,1,0,0),vec4f_t(0,0,1,0),vec4f_t(0,0,0,1));
    }
    mat4f_t(vec4f_t row0, vec4f_t row1, vec4f_t row2, vec4f_t row3){
        init(row0,row1,row2,row3);
    }

    mat4f_t operator *(float f)     const { return mat4f_t(m[0]*f,m[1]*f,m[2]*f,m[3]*f); }
    vec4f_t operator *(vec4f_t V)   const { return vec4f_t(m[0]*V,m[1]*V,m[2]*V,m[3]*V); }
    mat4f_t operator *(mat4f_t V)   const {
        mat4f_t retMat;
        for(int row=0;row<4;row++){
            for(int col=0;col<4;col++){
                retMat[row][col] = m[row]*vec4f_t(V.m[0][col], V.m[1][col], V.m[2][col], V.m[3][col]);
            }
        }
        return retMat; 
    }
    vec4f_t& operator[](const int i) {
        if(i<=3){return m[i];}
        else{throw "incorrect index input in mat4!";}
    }
    bool operator== (mat4f_t M)  const {
        for(int i=0;i<4;i++){
            if(M[i]!=m[i]){
                return false;
            } 
        }
        return true;
    }

    bool operator!= (mat4f_t M)  const {
        for(int i=0;i<4;i++){
            if(M[i]==m[i]){
                return false;
            } 
        }
        return true;
    }


    friend std::ostream& operator<<(std::ostream& out, mat4f_t& mat){
        out << "|" << mat.m[0][0] << ", "<<mat.m[0][1] << ", "<<mat.m[0][2] << ", "<<mat.m[0][3] << "|\n";
        out << "|" << mat.m[1][0] << ", "<<mat.m[1][1] << ", "<<mat.m[1][2] << ", "<<mat.m[1][3] << "|\n";
        out << "|" << mat.m[2][0] << ", "<<mat.m[2][1] << ", "<<mat.m[2][2] << ", "<<mat.m[2][3] << "|\n";
        out << "|" << mat.m[3][0] << ", "<<mat.m[3][1] << ", "<<mat.m[3][2] << ", "<<mat.m[3][3] << "|\n";
        return out;
    }

    float minor(int r, int c);
    float cofactor(int r, int c);

    mat4f_t adjoint();
    mat4f_t inverse();
    mat4f_t transpose();
    mat4f_t inverse_transpose();

};



//commonly used math function
template <class t> t lrMin(t t1, t t2){return t1 < t2 ? t1 : t2;}
template <class t> t lrMax(t t1, t t2){return t1 > t2 ? t1 : t2;}

template <class t> void lrSwap(t* t1, t* t2){ t temp = *t1; *t1 = *t2; *t2 = temp;}

template <class t> t lrLerp(t t1, t t2, float f){ return (t)(t1 + (t2-t1)*f); }

vec3f_t lrBarycentric(vec3f_t A, vec3f_t B, vec3f_t C, vec3f_t P);

#endif