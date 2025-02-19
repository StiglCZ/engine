#include "math.hh"
#include "renderer.hh"
#include "types.hh"
#include "native.hh"
typedef fx rvec[4];

const fx
    a = 1.0 / tan(rad(FOV) / 2.0),
    b = a / AR,
    c = (-NEAR - FAR) / AR,
    d = 2.0 * FAR * NEAR / AR;
    
matrix4x4 projMatrix = {
    {a, 0, 0, 0},
    {0, b, 0, 0},
    {0, 0, c, d},
    {0, 0, 1, 1},
};
matrix4x4 viewMatrix = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
};

// Matrix by Vector multiply
/* Readable implementation
   vector vec2;
   for(int n =0; n < 4; n++)
       vec2[n] = vec[n];
   for(int i =0, j =0; i < 4; i++){
        vec[i] = 0;
        for(j =0; j < 4; j++)
            vec[i] += mat[i][j] * vec2[j];
   }

   Insane performance

   What exactly is going on here?
   First, I convert the matrix into 16
   field array instead of 2D one, because
   its faster to access. Then, I unroll the
   loop for better readability, and then
   I use variable to only use 1 multiply
   and not 4, and then I multiply it to
   create dot point.

   And yes, I know there is a proper implementation of this
   but this is just faster, please do not edit this
   unless you somehow make it faster(still on the CPU tho)
*/

inline void MVm(fx* mat, Vector3 vec, rvec vec2) {
    #pragma unroll
    for(u8 i =0; i < 4; i++){
        u8 i4 = i * 4;
        vec2[i] =
            vec.X * mat[i4]     +   // X
            vec.Y * mat[i4 + 1] +   // Y
            vec.Z * mat[i4 + 2] +   // Z
            mat[i4 + 3];            // W(dst)
    }
}

void DrawPoly(Vector3* verts, u32 count, matrix4x4 matrix) {
    Point result[count];
    for(u32 i =0; i < count;i++){
        rvec vec;
        MVm((fx*)matrix, verts[i], vec);
        
        // Clipping
        if(
           fabs(vec[0]) > FOV        / 4 + 1 + vec[3] ||
           fabs(vec[1]) > (FOV * AR) / 4 + 1 + vec[3] ||
           // Ensures no objects behind the camera are being rendered
           vec[3] < CUTOFF_W
        )return;
        
        // Converts 4D objs back into 2D objs
        result[i].X = (int)(vec[0]  / (vec[3] * OVERHALFSIZE_X)) + HALFSIZE_X;
        result[i].Y = (int)(-vec[1] / (vec[3] * OVERHALFSIZE_Y)) + HALFSIZE_Y;
    }
    DrawLine(result[0], result[count-1]);
    for(u32 i = 1; i < count; i++)
        DrawLine(result[i-1], result[i]);
}

void DrawModel(Model* model, matrix4x4 matrix, Vector3* scale) {
    matrix4x4 temp1;
    matrixCombine(viewMatrix, matrix, temp1);
    matrixCombine(projMatrix, temp1, matrix);

    scale->Y *= AR2;
    for(u32 i =0; i < model->faces.size();i++){
        Face* f = &model->faces[i];
        Vector3 verts[f->count];
        for(u32 i =0; i < f->count; i++)
            verts[i] = model->verticies[f->verts[i]] * *scale;
        DrawPoly(verts, f->count, matrix);
    }
}
