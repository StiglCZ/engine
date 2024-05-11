#ifndef MATH
#define MATH
#include <cmath>
#ifdef _WIN32
#define M_PI 3.141592653589793
#endif

typedef float fx;
template <typename T> inline T max(T a, T b) { return (a > b) ? a : b; }
template <typename T> inline T min(T a, T b) { return (a < b) ? a : b; }
constexpr fx M_PIS180 = (M_PI / 180.0);
inline fx deg(fx src) { return src / M_PIS180; }
inline fx rad(fx src){ return src * M_PIS180; }
#endif
