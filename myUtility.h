#ifndef RTNEXTWEEK_H
#define RTNEXTWEEK_H

#include <random>
#include "vec3.h"
using namespace std;

// needed constant
#define M_PI 3.1415926535897932385
#define MY_INFINITY FLT_MAX
#define SIGMA 5.67e-8

inline float clamp(float x, float min, float max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

inline float degree_to_radians(float angle)
{
    return M_PI * angle / 180;
}

inline float random_float(default_random_engine &e)
{
    uniform_real_distribution<float> u(0, 1);
    return u(e);
}

// 获得单位圆（光圈）内随机的点
vec3 random_in_unit_disk(default_random_engine &e)
{
    vec3 p;
    while (true)
    {
        p = 2.0f * vec3(random_float(e), random_float(e), 0) - vec3(1, 1, 0);
        if (p.length_squared() >= 1.0f)
            continue;
        return p;
    }
}

inline float ffmin(float a, float b) { return a <= b ? a : b; }
inline float ffmax(float a, float b) { return a >= b ? a : b; }

#endif