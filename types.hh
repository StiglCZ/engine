#ifndef TYPES
#define TYPES
#include <vector>

typedef float fx;

struct Vector3 {
    fx X, Y, Z;
    friend Vector3 operator+(const Vector3&, const Vector3&);
    friend Vector3 operator-(const Vector3&, const Vector3&);
    friend Vector3 operator*(const Vector3&, const Vector3&);
    friend Vector3 operator/(const Vector3&, const Vector3&);
    friend Vector3 operator/(const Vector3&, const fx&);
    friend Vector3 operator*(const Vector3&, const fx&);
    friend Vector3 operator-(const Vector3&);
    friend void operator-=(Vector3&, const Vector3&);
    friend void operator+=(Vector3&, const Vector3&);
    friend void operator*=(Vector3&, const Vector3&);
    friend void operator/=(Vector3&, const Vector3&);
    friend void operator/=(Vector3&, const fx&);
    friend void operator*=(Vector3&, const fx&);
    friend bool operator==(const Vector3&, const Vector3&);
    friend bool operator!=(const Vector3&, const Vector3&);
    friend bool operator>(const Vector3 &, const Vector3&);
    friend bool operator<(const Vector3 &, const Vector3&);
    
    static const Vector3 UnitX;
    static const Vector3 UnitY;
    static const Vector3 UnitZ;
    static const Vector3 Zero;
    static const Vector3 One;

    fx   DistanceEuc(Vector3 other);
    fx   DistanceMan(Vector3 other);
    Vector3 Normalized();
    fx      Magnitude();
    Vector3 Forward();
    Vector3 Right();
    Vector3 Up();
};
struct Vector2 {
    fx X, Y;
    friend Vector2 operator+(const Vector2&, const Vector2&);
    friend Vector2 operator-(const Vector2&, const Vector2&);
    friend Vector2 operator*(const Vector2&, const Vector2&);
    friend Vector2 operator/(const Vector2&, const Vector2&);
    friend Vector2 operator/(const Vector2&, const fx&);
    friend Vector2 operator*(const Vector2&, const fx&);
    friend bool operator==(const Vector2&, const Vector2&);
    friend bool operator!=(const Vector2&, const Vector2&);

    static const Vector2 UnitX;
    static const Vector2 UnitY;
    static const Vector2 Zero;
    static const Vector2 One;
    
    fx   DistanceEuc(Vector2 other);
    fx   DistanceMan(Vector2 other);
    Vector2 Normalized();
    fx      Magnitude();

    Vector2 Right();
    Vector2 Up();
};

struct Point {
    int X, Y;
    friend Point operator*(const Point&, const Point&);
    friend Point operator/(const Point&, const Point&);
    friend Point operator*(const Point&, const int&);
    friend Point operator/(const Point&, const int&);
};

struct Color {
    unsigned short R, G, B;
    friend Color operator+(const Color&, const Color&);
};

struct Face {
    unsigned short* verts;
    unsigned int count;
};

typedef struct GameObject GameObject;
typedef struct GameData GameData;
typedef struct Trigger Trigger;
typedef struct Vector3 Vector3;
typedef struct Vector2 Vector2;
typedef struct Script Script;
typedef struct Color Color;
typedef struct Point Point;
typedef struct Model Model;
typedef struct Face Face;

typedef fx matrix4x4[4][4];
typedef fx matrix3x3[3][3];
typedef fx matrix2x2[2][2];

typedef fx matrix3x2[3][2];
typedef fx matrix2x3[2][3];

typedef unsigned long long u64;
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef long  i64;
typedef int   i32;
typedef short i16;
typedef char  i8;

typedef float  single;
typedef double f64;
typedef float  f32;

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

extern const GameObject emptyGameObj;

extern const Color WHITE;
extern const Color GRAY;
extern const Color BLACK;
extern const Color RED;
extern const Color GREEN;
extern const Color BLUE;
extern const Color YELLOW;
extern const Color PURPLE;
extern const Color CYAN;

struct Script {
    void (*init)(void* arg, u32 index);
    void (*start)(u32 index);
    void (*update)(u32 index);
    void* handle;
};

struct Model{
    std::vector<Vector3> verticies;
    std::vector<Face> faces;
    bool freed;
    u64 uId;
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
    u8 bounce;
};

struct CamProps {
    Vector3 pos, rot;
    void (*const sync)(CamProps*, bool);
    const fx fov, farp, nearp, ar, w, h;
};

struct GameData {
    std::vector<GameObject>  *const gameObjects;
    std::vector<void*>       *const exitFuncs;
    bool *const mouseCentered, *const isGameRunning;
    matrix4x4 *const viewMatrix, *const projMatrix;
    const u32 streamSize, sleepTime;
    u32 *const deltaTime, *const gameObjUID;
    u64 *const backColor;
    CamProps *const cp;
    Point *const mouse;
    u8 *const keys, *const btns, *const stream;
    u32 (*const loadModel ) (const char* name);
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
    RESOURCE_Model_Count = 5,
};

#ifndef _WIN32
#define memcpy(a, b, c)              \
    for(typeof(c) i = 0; i < c; i++) \
        a[i] = b[i];

#define strcpy(a, b)    \
    int i = 0;          \
    while (a[i])        \
        b[i] = a[i++];
#else
#include <cstring>
#define typeof(T) auto
#endif

#define fillMat(mat, pos, rot) \
    rotateW(mat, rot);         \
    mat[0][3] = pos.X;         \
    mat[1][3] = pos.Y;         \
    mat[2][3] = pos.Z;

#define printcol(col) std::cout << "[" << col.R << ", " << col.G << ", " << col.B << "]\n";
#define printvec3(v3) std::cout << '[' << v3.X << ", " << v3.Y << ", " << v3.Z << "]\n";

#define infocol(col)  Info('[' + std::to_string(col.R) + ", " + std::to_string(col.G) + ", " + std::to_string(v3.B) + "]");
#define infovec3(v3) Info('[' + std::to_string(v3.X) + ", " + std::to_string(v3.Y) + ", " + std::to_string(v3.Z) + "]");

#define drawtext(gd, text, x, y)gd->drawText({x, y}, text);
#define drawnum(gd, num, x, y)  gd->drawText({x, y}, std::to_string(text).c_str());
#define pushgo(gd, go) gd->gameObjects->push_back(go);

#define getDelta(gd) (gd->sleepTime > *gd->deltaTime) ? 1.0 : (fx)gd->sleepTime / *gd->deltaTime
#define getKey(gd, key) (u64)gd->getResource(4, (u64)key)
#define keyDown(gd, key) (gd->keys[keycode])
#define keyUp(gd, key)  !(gd->keys[keycode])

#define loadscene(gd, file) ((void(*)(const char*))*(void**)gd->stream)(file);
#define collisions(gd)((std::vector<u32> *)((void **)gd->stream)[2])
#define audioct(gd) (((AudioControl**)gd->stream)[3])
#define physics(gd) ((PhysicsProps*)((void**)gd->stream)[4])

#endif
