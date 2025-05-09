#include "types.hh"
#include <cmath>

// Vector3 utils
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
Vector3 operator-(const Vector3 &lhs) {
    return (Vector3){-lhs.X, -lhs.Y, -lhs.Z};
}
Vector3 operator+(const Vector3 &lhs) {
    return lhs;
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

fx Vector3::DistanceEuc(Vector3 other) {
    return std::sqrt(
        (X - other.X) * (X - other.X) +
        (Y - other.Y) * (Y - other.Y) +
        (Z - other.Z) * (Z - other.Z));
}
fx Vector3::DistanceMan(Vector3 other) {
    return
        std::fabs(X - other.X) +
        std::fabs(Y - other.Y) +
        std::fabs(Z - other.Z);
}

fx Vector3::Magnitude() { return std::abs(X) + std::abs(Y) + std::abs(Z); }
Vector3 Vector3::Normalized() {
    fx magnitude = Magnitude();
    if(magnitude < 1.0) return Zero;
    return *this / magnitude;
}

Vector3 Vector3::Forward() {
    return {
        (float)(+cos(X) * sin(Y)),
        (float)(-sin(X)),
        (float)(+cos(X) * -cos(Y)),
    };
}

Vector3 Vector3::Right() {
    return {
        (float)cos(Y),
        (float)0,
        (float)sin(Y),
    };
}

Vector3 Vector3::Up() {
    return{
        (float)(sin(X) * sin(Y)),
        (float)(cos(X)),
        (float)(sin(X) * cos(Y)),
    };
}

const Vector3 Vector3::UnitX = {1, 0, 0};
const Vector3 Vector3::UnitY = {0, 1, 0};
const Vector3 Vector3::UnitZ = {0, 0, 1};
const Vector3 Vector3::Zero  = {0, 0, 0};
const Vector3 Vector3::One   = {1, 1, 1};

// Vector2 utils
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

fx Vector2::DistanceEuc(Vector2 other) {
    return std::sqrt(
        (X - other.X) * (X - other.X) +
        (Y - other.Y) * (Y - other.Y));
}
fx Vector2::DistanceMan(Vector2 other) {
    return
        std::fabs(X - other.X) +
        std::fabs(Y - other.Y);
}

fx Vector2::Magnitude() { return std::abs(X) + std::abs(Y); }
Vector2 Vector2::Normalized() {
    fx magnitude = Magnitude();
    if(magnitude < 1.0) return Zero;
    return *this / magnitude;
}

Vector2 Vector2::Right() {
    return {
        (fx)cos(Y),
        (fx)sin(Y),
    };
}

Vector2 Vector2::Up() {
    return{
        (fx)sin(Y),
        (fx)cos(Y),
    };
}

const Vector2 Vector2::UnitX = {1, 0};
const Vector2 Vector2::UnitY = {0, 1};
const Vector2 Vector2::Zero  = {0, 0};
const Vector2 Vector2::One   = {1, 1};

// Point utils
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

// Color utils
Color operator+(const Color &lhs, const Color &rhs) {
    return (Color){(u16)(lhs.R + rhs.R), (u16)(lhs.G + rhs.G), (u16)(lhs.B + rhs.B)};
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

void matrixCopy(fx* a, const fx* b, Point src, int dstX) {
    for(int i =0; i < src.Y; i++)
        for(int j =0; j < src.X; j++)
            a[i * dstX + j] = b[i * src.X + j];
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
void rotateW(matrix3x3 out, const Vector3 w) {
    matrix3x3 result1 = {}, result2 = {}, result3 = {};
    
    rotateZ(result1, w.Z);
    rotateY(result2, w.Y);
    matrixCombine(result1, result2, result3);

    rotateX(result1, w.X);
    matrixCombine(result3, result1, out);
}
void rotateW(matrix4x4 out, const Vector3 w) {
    matrix3x3 result1, result2, result3 = {};
    
    rotateZ(result1, w.Z);
    rotateY(result2, w.Y);
    matrixCombine(result1, result2, result3);
    
    rotateX(result1, w.X);
    matrixCombine(result3, result1, result2);
    matrixCopy(out, result2);
    matrixCopy(out, out);
}

const Color WHITE = {255, 255, 255};
const Color GRAY  = {127, 127, 127};
const Color BLACK = {000, 000, 000};
const Color RED   = {255, 000, 000};
const Color GREEN = {000, 255, 000};
const Color BLUE  = {000, 000, 255};

const Color YELLOW = {255, 255, 000};
const Color PURPLE = {255, 000, 255};
const Color CYAN   = {000, 255, 255};

const GameObject emptyGameObj = {
    .model     = 0,
    .script    = 0,
    .colliding = 0,
    .flags     = 0,
    .id        = 0,
    .color     = BLACK,
    
    .position = {0, 0, 0},
    .rotation = {0, 0, 0},
    .scale    = {1, 1, 1},
    .coll     = {0, 0, 0},
    .velocity = {0, 0, 0},
    .bounce   = 0x0000000,
};
