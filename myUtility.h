#ifndef MY_UTILITY_H
#define MY_UTILITY_H

#include <random>
#include "vec3.h"
using namespace std;

// needed constant
#define MY_PI 3.1415926535897932385
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
    return MY_PI * angle / 180;
}

inline float random_float(default_random_engine &e)
{
    uniform_real_distribution<float> u(0, 1);
    return u(e);
}

// 获得单位圆（光圈）内随机的点
inline vec3 random_in_unit_disk(default_random_engine &e)
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

// 获得半球表面的满足概率密度p=cos/pi的随机向，因为不仅可以用于兰贝特反射向量的计算，所以抽象出来
inline vec3 random_cosine_direction(default_random_engine& e)
{
    float r1 = random_float(e);
    float r2 = random_float(e);
    float z = sqrt(1 - r2);

    float phi = 2 * MY_PI * r1;
    float x = cos(phi) * sqrt(r2);
    float y = sin(phi) * sqrt(r2);

    return vec3(x, y, z);
}

inline float ffmin(float a, float b) { return a <= b ? a : b; }
inline float ffmax(float a, float b) { return a >= b ? a : b; }

#endif