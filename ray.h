/* 
 * this file define the ray(viewing-ray...) incldue emit-point(orig) and direction(dir) 
 * and a function : vec3 at(double t) const; to get it's state at time t 
 *
 * 
 */
#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include <iostream>

using namespace std;

class ray {
public:
    ray() {}
    ray(const vec3 &origin, const vec3 &direction, float time = 0.0) : 
                orig(origin), dir(direction), tm(time) { }

    vec3 origin() const { return orig; }
    vec3 direction() const { return dir; }
    float time() const { return tm; }

    vec3 at(float t) const 
    {
        return orig + t * dir;
    }

public: 
    vec3 orig;
    vec3 dir;
    float tm;
};

#endif