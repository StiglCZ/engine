#include <cmath>
#include "types.hh"
void RotationX(matrix3x3 mat, double w) {
    double a = 1;
    double b = cos(w);
    double c = -sin(w);
    double d = cos(w);
    double e = sin(w);
    mat[0][0] = a;
    mat[0][1] = 0;
    mat[0][2] = 0;
    
    mat[1][0] = 0;
    mat[1][1] = b;
    mat[1][2] = c;
    
    mat[2][0] = 0;
    mat[2][1] = e;
    mat[2][2] = d;
}

void RotationY(matrix3x3 mat, double w) {
    double a = cos(w);
    double b = sin(w);
    double c = 1;

    double d = -sin(w);
    double e = cos(w);
    mat[0][0] = a;
    mat[0][1] = 0;
    mat[0][2] = b;

    mat[1][0] = 0;
    mat[1][1] = c;
    mat[1][2] = 0;
    
    mat[2][0] = d;
    mat[2][1] = 0;
    mat[2][2] = e;
}
void RotationZ(matrix3x3 mat, double w) {
    double a = cos(w);
    double b = -sin(w);
    
    double c = sin(w);
    double d = cos(w);
    double e = 1;
    mat[0][0] = a;
    mat[0][1] = b;
    mat[0][2] = 0;
    
    mat[1][0] = c;
    mat[1][1] = d;
    mat[1][2] = 0;

    mat[2][0] = 0;
    mat[2][1] = 0;
    mat[2][2] = e;
}
