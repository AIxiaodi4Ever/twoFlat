#ifndef AARECT_H
#define AARECT_H

#include "hittable.h"
#include "material.h"

// 平行于xy的矩形
class xy_rect : public hittable
{
public:
    xy_rect() {}
    xy_rect(material *mat, float _x0, float _x1, float _y0, float _y1, float _k, float T) :
        mat_ptr(mat), x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), temperature(T) {}

    ~xy_rect() { delete mat_ptr; }

    virtual bool hit(const ray &r, float t0, float t1, hit_record &rec) const;

public:
    material *mat_ptr;
    float x0, x1, y0, y1, k;
    float temperature;
};

// 平行于xz的矩形
class xz_rect : public hittable
{
public:
     xz_rect() {}
     xz_rect(material *mat, float _x0, float _x1, float _z0, float _z1, float _k, float T) :
        mat_ptr(mat), x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), temperature(T) {}

     ~xz_rect() { delete mat_ptr; }

     virtual bool hit(const ray &r, float t0, float t1, hit_record &rec) const;

public:
    material *mat_ptr;
    float x0, x1, z0, z1, k;
    float temperature;
};

class yz_rect : public hittable
{
public:
     yz_rect() {}
     yz_rect(material *mat, float _y0, float _y1, float _z0, float _z1, float _k, float T) :
        mat_ptr(mat), y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), temperature(T) {}

     ~yz_rect() { delete mat_ptr; }

     virtual bool hit(const ray &r, float t0, float t1, hit_record &rec) const;

public:
    material *mat_ptr;
    float y0, y1, z0, z1, k;
    float temperature;
};

#endif