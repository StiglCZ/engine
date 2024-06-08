#ifndef MATH
#define MATH
#include <cmath>
#ifndef __unix__
#define M_PI 3.14159265358979323846
#endif
template <typename T> inline T max(T a, T b) { return (a > b) ? a : b; }
template <typename T> inline T min(T a, T b) { return (a < b) ? a : b; }
typedef float fx;
constexpr fx M_PID180 = (M_PI / 180.0);
inline fx deg(fx src) { return src / M_PID180; }
inline fx rad(fx src){ return src * M_PID180; }
#endif
