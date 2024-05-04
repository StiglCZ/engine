#ifndef TYPES
#define TYPES
#include <vector>

typedef float fx;

struct Vector3 {
    fx X, Y, Z;
    friend Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
    friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
    friend Vector3 operator*(const Vector3& lhs, const Vector3& rhs);
    friend Vector3 operator/(const Vector3& lhs, const Vector3& rhs);
    friend Vector3 operator/(const Vector3& lhs, const fx& rhs);
    friend Vector3 operator*(const Vector3& lhs, const fx& rhs);
    friend void operator-=(Vector3& lhs, const Vector3& rhs);
    friend void operator+=(Vector3& lhs, const Vector3& rhs);
    friend void operator*=(Vector3& lhs, const Vector3& rhs);
    friend void operator/=(Vector3& lhs, const Vector3& rhs);
    friend void operator/=(Vector3& lhs, const fx& rhs);
    friend void operator*=(Vector3& lhs, const fx& rhs);
    friend bool operator==(const Vector3& lhs, const Vector3& rhs);
    friend bool operator!=(const Vector3& lhs, const Vector3& rhs);
    friend bool operator>(const Vector3 &lhs, const Vector3 &rhs);
    friend bool operator<(const Vector3 &lhs, const Vector3 &rhs);
};

struct Vector2 {
    fx X, Y;
    friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
    friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
    friend Vector2 operator*(const Vector2& lhs, const Vector2& rhs);
    friend Vector2 operator/(const Vector2& lhs, const Vector2& rhs);
    friend Vector2 operator/(const Vector2& lhs, const fx& rhs);
    friend Vector2 operator*(const Vector2& lhs, const fx& rhs);
    friend bool operator==(const Vector2& lhs, const Vector2& rhs);
    friend bool operator!=(const Vector2& lhs, const Vector2& rhs);
};


struct Point {
    int X, Y;
    friend Point operator*(const Point& lhs, const Point& rhs);
    friend Point operator/(const Point& lhs, const Point& rhs);
    friend Point operator/(const Point& lhs, const int& rhs);
    friend Point operator*(const Point& lhs, const int& rhs);
};

struct Color {
    unsigned short R, G, B;
    friend Color operator+(const Color& lhs, const Color& rhs);
};

struct Face {
    unsigned short* m;
    unsigned int count;
};

struct Camera {
    Vector3 pos, rot;
};

struct Portal {
    Vector3 pos, rot;
    Vector2 scale;
};

struct PortCamPair {
    Camera cam;
    Portal port;
};

typedef struct GameObject GameObject;
typedef struct GameData GameData;
typedef struct Trigger Trigger;
typedef struct Vector3 Vector3;
typedef struct Vector2 Vector2;
typedef struct Script Script;
typedef struct Color color;
typedef struct Point Point;
typedef struct Model Model;
typedef struct Face Face;

typedef fx matrix4x4[4][4];
typedef fx matrix3x3[3][3];
typedef fx matrix2x2[2][2];

typedef fx matrix3x2[3][2];
typedef fx matrix2x3[2][3];

typedef unsigned long  u64;
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef long  i64;
typedef int   i32;
typedef short i16;
typedef char  i8;
typedef bool  i1;

typedef double f64;
typedef float  f32;
typedef float  single;

const matrix4x4 matIdentify = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
};
const matrix4x4 modelMat = {
    {1, 1, 1, 0},
    {1, 1, 1, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 1},
};

// types.o required
extern void matrixCombine(const matrix3x3 a, const matrix3x3 b, matrix3x3 c);
extern void matrixCombine(const matrix4x4 a, const matrix4x4 b, matrix4x4 c);

extern void matrixCopy(fx*       a, const fx*       b, Point src, u32 dstX);
extern void matrixCopy(matrix3x3 a, const matrix3x3 b);
extern void matrixCopy(matrix4x4 a, const matrix3x3 b);
extern void matrixCopy(matrix4x4 a, const matrix4x4 b);

extern void flipMatrix(fx *mat, u16 size);
extern void flipMatrix4x4(matrix4x4 mat);
extern void flipMatrix3x3(matrix3x3 mat);

extern void rotateX(matrix3x3 out, fx w);
extern void rotateY(matrix3x3 out, fx w);
extern void rotateZ(matrix3x3 out, fx w);
extern void rotateW(matrix3x3 out, Vector3 w);
extern void rotateW(matrix4x4 out, Vector3 w);

extern fx distance(Vector3 a, Vector3 b);
extern fx magnitude(Vector3 a);

extern const Color WHITE;
extern const Color GRAY;
extern const Color BLACK;
extern const Color RED;
extern const Color GREEN;
extern const Color BLUE;

extern const GameObject emptyGameObj;

struct Script {
    void (*init)(void* arg, u32 scriptIndex);
    void (*start)(u32 index);
    void (*update)(u32 index);
    void* handle;
};

struct Model{
    std::vector<Vector3> verticies;
    std::vector<Face> faces;
    Vector3 bias;
    u64 uId;
    i1 freed;
};

struct GameObject {
    u32 model, script, colliding = 0;
    u64 flags, id;
    Color color = {};
    Vector3
        position = {},
        rotation = {},
        scale = {},
        coll = {},
        velocity = {};
};

struct CamProps {
    Vector3 pos, rot;
    void (*const sync)(CamProps*);
    const fx fov, far, near;
};

struct GameData {
    std::vector<PortCamPair> *const portCamPairs;
    std::vector<GameObject>  *const gameObjects;
    std::vector<void*>       *const exitFuncs;
    bool *const mouseCentered, *const isGameRunning;
    matrix4x4 *const viewMatrix, *const projMatrix;
    const u32 streamSize, W, H;
    const fx AR;
    u32 *const deltaTime, *const gameObjUID;
    CamProps *const cp;
    Point *const mouse;
    u8 *const keys, *const btns, *const stream;
    u32 (*const loadObject) (const char* name);
    u32 (*const loadScript) (const char* name, void* gd);
    void  (*const drawText) (Point pos, const char* str);
    void  (*const screenShot) (const char* filename);
    void* (*const getResource)  (u8 type, u64 uID);
};

enum ResourceType {
    RESOURCE_Zero = 0,
    RESOURCE_Script = 1,
    RESOURCE_Model_by_permanent_id = 2,
    RESOURCE_Model_by_temporary_id= 3,
    RESOURCE_Keycode = 4,
};

#define memcpy(a, b, c)              \
    for(typeof(c) i = 0; i < c; i++) \
        a[i] = b[i];

#define strcpy(a, b)    \
    int i = 0;          \
    while (a[i])        \
        b[i] = a[i++];

#define printvec3(v3)                                                   \
    std::cout << '[' << v3.X << ", " << v3.Y << ", " << v3.Z << "]\n";
#define infovec3(v3)                                                    \
    Info('[' + std::to_string(v3.X) + ", " + std::to_string(v3.Y) + ", " + std::to_string(v3.Z) + "]\n");

#define getKey(gd, key) \
    (u64)gd->getResource(4, (u64)key)

#define fillMat(mat, pos, rot)                                          \
    rotateW(mat, rot);                                                  \
    mat[0][3] = pos.X;                                                  \
    mat[1][3] = pos.Y;                                                  \
    mat[2][3] = pos.Z;

#endif
