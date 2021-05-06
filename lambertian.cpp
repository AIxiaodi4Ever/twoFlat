#include "material.h"

vec3 lambertian::emitted(const ray &r_in, const hit_record &rec, float u, float v, const vec3 &p) const
{
        float Eb = SIGMA * pow(rec.temperature, 4);
        vec3 E = (vec3(1.0, 1.0, 1.0) - albedo) * Eb;
        vec3 Ib = E / MY_PI;
        return Ib;
}

bool lambertian::scatter(const ray &r_in, const hit_record &rec, scatter_record& srec, default_random_engine &e) const
{
    srec.is_specular = false;
    srec.attenuation = albedo;
    srec.pdf_ptr = new cosine_pdf(rec.normal);
    return true;
}