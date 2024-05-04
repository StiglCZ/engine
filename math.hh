#ifndef MATH
#define MATH
#include <cmath>
#define DTRAD M_PI / 180.0
#define RTDEG 1.0 / (M_PI / 180.0)
/*
#define M_PI 3.14159265358979323846
extern fx tan(fx source);
extern fx sin(fx source);
extern fx cos(fx source);
extern fx degToRad(fx src);
extern fx fmod(fx x, fx y);
*/
typedef float fx;
template <typename T> inline T max(T a, T b) { return (a > b) ? a : b; }
template <typename T> inline T min(T a, T b) { return (a < b) ? a : b; }
inline fx degToRad(fx src) { return src * (M_PI / 180.0); }
constexpr fx M_PIS180 = (M_PI / 180.0);
inline fx deg(fx src) { return src / M_PIS180; }
inline fx rad(fx src){ return src * M_PIS180; }
#endif
