#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "hittable.h"
#include "myUtility.h"
#include "onb.h"
#include "pdf.h"

// 菲涅尔公式的Christophe Schlick近似，获得不同入射角下的反射率
inline float schlick(float cosine, float ref_idx)
{
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 * (1 - r0) * pow(1 - cosine, 5);
}

// 获得电解质中的折射向量
inline bool refract(const vec3& v, const vec3& n, float etai_over_etat, vec3& refracted)
{
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0f - etai_over_etat*etai_over_etat*(1-dt*dt);
    if (discriminant > 0) {
        refracted = etai_over_etat*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else
        return false;
}

// 返回镜反射向量
// 无论法向量指向内或外都能得到正确的反射方向
inline vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2.0f * dot(v, n) * n;
}

struct scatter_record {
    ray specular_ray;
    bool is_specular;
    vec3 attenuation;
    pdf *pdf_ptr;
};

class material {
public:
    virtual ~material(){}

    virtual vec3 emitted(const ray &r_in, const hit_record &rec, float u, float v, const vec3 &p) const = 0;

    // 这里认为光线传播的速度为无穷大所以接触时光线的时间与反射后的时间都等于光线最初发射的时间
    virtual bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec, default_random_engine &e) const = 0;

};

// 服从兰贝特分布的表面
class lambertian : public material {
public:
    lambertian(vec3 a) : albedo(a) {}

    ~lambertian() { }

    virtual vec3 emitted(const ray &r_in, const hit_record &rec, float u, float v, const vec3 &p) const;

    virtual bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec, default_random_engine &e) const;


public:
    vec3 albedo;
};

#endif