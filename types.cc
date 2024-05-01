#include "types.hh"
#include <cmath>

Vector3 operator+(const Vector3 &lhs, const Vector3 &rhs) {
    return (Vector3){lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z};
}
Vector3 operator-(const Vector3 &lhs, const Vector3 &rhs) {
    return (Vector3){lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z};
}
Vector3 operator*(const Vector3 &lhs, const Vector3 &rhs) {
    return (Vector3){lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z};
}
Vector3 operator/(const Vector3 &lhs, const Vector3 &rhs) {
    return (Vector3){lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z};
}

Vector3 operator/(const Vector3 &lhs, const fx &rhs) {
    return (Vector3){lhs.X / rhs, lhs.Y / rhs, lhs.Z / rhs};
}
Vector3 operator*(const Vector3 &lhs, const fx &rhs) {
    return (Vector3){lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs};
}
void operator-=(Vector3& lhs, const Vector3& rhs) {
  lhs.X -= rhs.X;
  lhs.Y -= rhs.Y;
  lhs.Z -= rhs.Z;
}
void operator+=(Vector3& lhs, const Vector3& rhs) {
  lhs.X += rhs.X;
  lhs.Y += rhs.Y;
  lhs.Z += rhs.Z;
}
void operator*=(Vector3& lhs, const Vector3& rhs) {
  lhs.X *= rhs.X;
  lhs.Y *= rhs.Y;
  lhs.Z *= rhs.Z;
}
void operator/=(Vector3& lhs, const Vector3& rhs) {
  lhs.X /= rhs.X;
  lhs.Y /= rhs.Y;
  lhs.Z /= rhs.Z;
}
void operator/=(Vector3& lhs, const fx& rhs) {
  lhs.X /= rhs;
  lhs.Y /= rhs;
  lhs.Z /= rhs;
}
void operator*=(Vector3& lhs, const fx& rhs) {
  lhs.X *= rhs;
  lhs.Y *= rhs;
  lhs.Z *= rhs;
}


bool operator==(const Vector3 &lhs, const Vector3 &rhs) {
    return (lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z);
}
bool operator!=(const Vector3 &lhs, const Vector3 &rhs) {
    return !(lhs == rhs);
}
bool operator>(const Vector3 &lhs, const Vector3 &rhs) {
    return (lhs.X > rhs.X && lhs.Y > rhs.Y && lhs.Z > rhs.Z);
}
bool operator<(const Vector3 &lhs, const Vector3 &rhs) {
    return (lhs.X < rhs.X && lhs.Y < rhs.Y && lhs.Z < rhs.Z);
}

Vector2 operator+(const Vector2 &lhs, const Vector2 &rhs) {
    return (Vector2){lhs.X + rhs.X, lhs.Y + rhs.Y};
}
Vector2 operator-(const Vector2 &lhs, const Vector2 &rhs) {
    return (Vector2){lhs.X - rhs.X, lhs.Y - rhs.Y};
}
Vector2 operator*(const Vector2 &lhs, const Vector2 &rhs) {
    return (Vector2){lhs.X * rhs.X, lhs.Y * rhs.Y};
}
Vector2 operator/(const Vector2 &lhs, const Vector2 &rhs) {
    return (Vector2){lhs.X / rhs.X, lhs.Y / rhs.Y};
}

Vector2 operator/(const Vector2 &lhs, const fx &rhs) {
    return (Vector2){lhs.X / rhs, lhs.Y / rhs};
}
Vector2 operator*(const Vector2 &lhs, const fx &rhs) {
    return (Vector2){lhs.X * rhs, lhs.Y * rhs};
}

bool operator==(const Vector2 &lhs, const Vector2 &rhs) {
    return (lhs.X == rhs.X && lhs.Y == rhs.Y);
}
bool operator!=(const Vector2 &lhs, const Vector2 &rhs) {
    return !(lhs == rhs);
}

Point operator*(const Point &lhs, const Point &rhs) {
    return (Point){lhs.X * rhs.X, lhs.Y * rhs.Y};
}
Point operator/(const Point &lhs, const Point &rhs) {
    return (Point){lhs.X / rhs.X, lhs.Y / rhs.Y};
}

Point operator*(const Point &lhs, const int &rhs) {
    return {lhs.X * rhs, lhs.Y * rhs};
}
Point operator/(const Point &lhs, const int &rhs) {
    return {lhs.X / rhs, lhs.Y / rhs};
}

fx distance(const Vector3 a, const Vector3 b) {
    return
        std::fabs(a.X - b.X) +
        std::fabs(a.Y - b.Y) +
        std::fabs(a.Z - b.Z);
}

void matrixCombine(const matrix3x3 a, const matrix3x3 b, matrix3x3 c) {
    #pragma unroll
    for(int i =0; i < 3;i++)
        #pragma unroll
        for(int n =0; n < 3; n++){
            c[i][n] =
                a[i][0] * b[0][n] +
                a[i][1] * b[1][n] +
                a[i][2] * b[2][n];
        }
}
void matrixCombine(const matrix4x4 a, const matrix4x4 b, matrix4x4 c) {
    #pragma unroll
    for(int i =0; i < 4;i++)
        #pragma unroll
        for(int n =0; n < 4; n++){
            c[i][n] =
                a[i][0] * b[0][n] +
                a[i][1] * b[1][n] +
                a[i][2] * b[2][n] +
                a[i][3] * b[3][n];
        }
}
void matrixCopy(matrix4x4 a, const matrix4x4 b) {
    for(int i =0; i < 4; i++)
        for(int j = 0; j < 4;j++)
            a[i][j] = b[i][j];
}
void matrixCopy(matrix3x3 a, const matrix3x3 b) {
    #pragma unroll
    for(int i =0; i < 3; i++)
        #pragma unroll
        for(int j = 0; j < 3;j++)
            a[i][j] = b[i][j];
}
void matrixCopy(matrix4x4 a, const matrix3x3 b) {
    #pragma unroll
    for(int i =0; i < 3; i++)
        #pragma unroll
        for(int j = 0; j < 3;j++)
            a[i][j] = b[i][j];
}
void matrixCopy(fx* a, const fx* b, Point src, int dstX) {
    for(int i =0; i < src.Y; i++)
        for(int j =0; j < src.X; j++)
            a[i * dstX + j] = b[i * src.X + j];
}
void rotateX(matrix3x3 out, fx w) {
    fx* oout = (fx*)out;
    fx a = 1;
    fx b = cos(w);
    fx c = -sin(w);
    fx d = sin(w);
    fx e = cos(w);
    oout[0] = a;
    oout[1] = 0;
    oout[2] = 0;
    
    oout[3] = 0;
    oout[4] = b;
    oout[5] = c;
    
    oout[6] = 0;
    oout[7] = d;
    oout[8] = e;
}
void rotateY(matrix3x3 out, fx w) {
    fx* oout = (fx*)out;
    fx a = cos(w);
    fx b = sin(w);
    fx c = 1;

    fx d = -sin(w);
    fx e = cos(w);
    oout[0] = a;
    oout[1] = 0;
    oout[2] = b;

    oout[3] = 0;
    oout[4] = c;
    oout[5] = 0;
    
    oout[6] = d;
    oout[7] = 0;
    oout[8] = e;
}
void rotateZ(matrix3x3 out, fx w) {
    fx* oout = (fx*)out;
    fx a = cos(w);
    fx b = -sin(w);
    
    fx c = sin(w);
    fx d = cos(w);
    fx e = 1;
    oout[0] = a;
    oout[1] = b;
    oout[2] = 0;
    
    oout[3] = c;
    oout[4] = d;
    oout[5] = 0;

    oout[6] = 0;
    oout[7] = 0;
    oout[8] = e;
}

void flipMatrix(fx* mat, u16 size) {
    u16 sSq = size * size;
    fx old[sSq];
    memcpy(old, mat, sSq);
    for(int i =0; i < size; i++)
        for(int j =0; j < size; j++)
            mat[j * size + i] = old[i * size + j];
}
void flipMatrix3x3(matrix3x3 mat) {
    matrix3x3 old;
    matrixCopy(old, mat);
    for(int i =0; i < 3; i++)
        for(int j =0; j < 3; j++)
            mat[j][i] = old[i][j];
}
void flipMatrix4x4(matrix4x4 mat) {
    matrix4x4 old;
    matrixCopy(old, mat);
    for(int i =0; i < 4; i++)
        for(int j =0; j < 4; j++)
            mat[j][i] = old[i][j];
}

void rotateW(matrix3x3 out, const Vector3 w) {
    matrix3x3 result1 = {}, result2 = {}, result3 = {};
    
    rotateZ(result1, w.Z);
    rotateY(result2, w.Y);
    matrixCombine(result1, result2, result3);

    rotateX(result1, w.X);
    matrixCombine(result3, result1, out);
    /*Somewhat works, but make it 0width
    fx a = w.Z;
    fx b = w.Y;
    fx c = w.X;
    fx sa = sin(a), ca = cos(a);
    fx sb = sin(b), cb = cos(b);
    fx sc = sin(c), cc = cos(c);
    fx* o = (fx*)out;
    o[0] = ca * cb;
    o[3] = sa * cb;
    o[6] = - sb;

    o[1] = ca * sb * sc - sa * cc;
    o[4] = sa * sb * sc + ca * cc;
    o[7] = cb * sc;

    o[2] = ca * sb * cc + sa * sb;
    o[5] = sa * sb * cc - ca * sb;
    o[8] = cb * cc;
    */
}
void rotateW(matrix4x4 out, const Vector3 w) {
    matrix3x3 result1 = {}, result2 = {}, result3 = {};
    
    rotateZ(result1, w.Z);
    rotateY(result2, w.Y);
    matrixCombine(result1, result2, result3);
    
    rotateX(result1, w.X);
    matrixCombine(result3, result1, result2);
    matrixCopy(out, result2);
    matrixCopy(out, out);
}
fx magnitude(const Vector3 a) {
    return fabs(a.X) + fabs(a.Y) + fabs(a.Z);
}

const GameObject emptyGameObj = {
    0, 0, 0, 0, 0,
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {1, 1, 1},
    {0, 0, 0},
    {0, 0, 0}
};
