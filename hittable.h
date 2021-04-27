/*
 * this file define the father class of any hittable object 
 * and define a real virtual function hit() to provide a common interface
 */

#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "material.h"

class material;

struct hit_record
{
    // 这里的t是步长数，前进了t倍的方向向量ray::dir后与某物体碰撞
    float t;
    float u;    // u,v存储像素位置占像素总数的百分数
    float v;

    float temperature;  // 碰撞点温度 单位：K

    vec3 p;
    bool front_face;
    vec3 normal;
    material *mat_ptr;


    void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    hittable() { };
    hittable(material *m) : mat_ptr(m) {  }
    virtual ~hittable() { delete mat_ptr; }
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;     

public:
    // material的指针移动到hittable里，否则global函数free_word无法正确清除指针
    material *mat_ptr;
};

// 将front_face取反，这样原本设置的面向x，y，z的正方向的面现在被认为面向x，y，z的负方向
// 在diffuse_light类中，用于设置光源只向front_face为true的面发光
class flip_face : public hittable
{
public:
    flip_face(hittable *p) : ptr(p) {}

    ~flip_face() { delete ptr; }

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const
    {
        if (!ptr->hit(r, t_min, t_max, rec))
            return false;

        rec.front_face = !rec.front_face;
        return true;
    }

public:
    hittable *ptr;
};


#endif