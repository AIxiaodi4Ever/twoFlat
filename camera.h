/* this file define camera class from camera we can get ray */
#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "myUtility.h"

// 视线是逆着摄像机坐标系(相对坐标系uvw)的w方向，即-w方向追踪的，目的是使取景框的“左下角”lower_left_corner和图像的左下角一致
class camera {
public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, 
                    float aperture, float focus_dist, float t0 = 0.0, float t1 = 0.0) 
    { // vfov is top to bottom in degrees
        lens_radius = aperture / 2.0f;
        time0 = t0;
        time1 = t1;
        float theta = vfov*((float)MY_PI)/180.0f;
        float half_height = tan(theta/2.0f);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));     //vup取任意不和w平行的向量即可
        v = cross(w, u);
        lower_left_corner = origin  - half_width*focus_dist*u -half_height*focus_dist*v - focus_dist*w;
        horizontal = 2.0f*half_width*focus_dist*u;
        vertical = 2.0f*half_height*focus_dist*v;
    }

    ray get_ray(float s, float t, default_random_engine& e) 
    {
        vec3 rd = lens_radius*random_in_unit_disk(e);
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin + offset, 
                   lower_left_corner + s*horizontal + t*vertical - origin - offset,
                   (time1 - time0) * random_float(e) + time0);
    }

public:
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
    float time0, time1;         // shutter open/close times
};

#endif