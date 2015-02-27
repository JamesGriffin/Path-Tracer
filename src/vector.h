#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <math.h>

struct Vec {
    double x, y, z;

    Vec(double x_=0, double y_=0, double z_=0) : x(x_), y(y_), z(z_) {}

    double axis(uint32_t axis){
        if (axis == 0) return x;
        if (axis == 1) return y;
        if (axis == 2) return z;
    }

    // Vector operations
    Vec operator+(const Vec &b) const { return Vec(x+b.x,y+b.y,z+b.z); }
    Vec operator-(const Vec &b) const { return Vec(x-b.x,y-b.y,z-b.z); }
    Vec operator*(double b) const { return Vec(x*b,y*b,z*b); }
    Vec operator/(float b) const { return Vec(x/b,y/b,z/b); }
    Vec mult(const Vec &b) const { return Vec(x*b.x,y*b.y,z*b.z); }
    Vec& norm(){ return *this = *this * (1/sqrt(x*x+y*y+z*z)); }
    double dot(const Vec &b) const { return x*b.x+y*b.y+z*b.z; } // cross:
    Vec operator%(Vec&b){return Vec(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);}
    Vec cross(const Vec&b){return Vec(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);}
    double mag() const{return sqrt(x*x+y*y+z*z);}
};

#endif // VECTOR_H