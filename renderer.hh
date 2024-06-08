#ifndef RENDERER
#define RENDERER

#include "types.hh"

#define FOV 80.0
#define SIZE_X 1280
#define SIZE_Y 720
#define PFAR 100.0
#define PNEAR 0.01

#define CUTOFF_Z 0
#define CUTOFF_W 0
constexpr fx HALFSIZE_X = SIZE_X / 2.0,
             HALFSIZE_Y = SIZE_Y / 2.0,
             OVERHALFSIZE_X = 1.0 / HALFSIZE_X,
             OVERHALFSIZE_Y = 1.0 / HALFSIZE_Y,
             AR = (float)SIZE_X / SIZE_Y,
             AR2 = AR * AR;

extern matrix4x4 viewMatrix;
extern matrix4x4 projMatrix;

extern void DrawModel(Model *model, matrix4x4 matrix, Vector3 *scale);
extern void MatrixFromPortal(Vector3 camera_pos, Vector3 camera_rot,
                      Vector3 portal_pos, Vector3 portal_rot,
                      Vector3 portac_pos, Vector3 portac_rot,
                      matrix4x4 target);
#endif
