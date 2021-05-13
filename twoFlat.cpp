#include <iostream>
#include <time.h>
#include <float.h>
#include <fstream>
#include <string>

#include "vec3.h"
#include "ray.h"
#include "aarect.h"
#include "hittableList.h"
#include "camera.h"
#include "material.h"
#include "pdf.h"

using namespace std;

vec3 ray_color(const ray& r, const vec3& background, hittable** d_world, default_random_engine& e) 
{
    ray cur_ray = r;
    vec3 cur_val = vec3(0, 0, 0);
    vec3 cur_attenuation = vec3(1.0, 1.0, 1.0);
    scatter_record srec;
    for (int i = 0; i < 50; ++i) {
        hit_record rec;
        if ((*d_world)->hit(cur_ray, 0.001f, FLT_MAX, rec)) {
            vec3 emitted = rec.mat_ptr->emitted(cur_ray, rec, rec.u, rec.v, rec.p);
            cur_val += (cur_attenuation * emitted);
            if (rec.mat_ptr->scatter(cur_ray, rec, srec, e)) {
                cur_attenuation = cur_attenuation * srec.attenuation;
                ray scattered = ray(rec.p, srec.pdf_ptr->generate(e), cur_ray.time());
                // !!! this pointer must delete !!!
                delete srec.pdf_ptr;
                cur_ray = scattered;
            }
        }
        else {
            return cur_val;
        }
    }
    //cerr << cur_val << endl;
    return cur_val;
}

void render(vec3 *fb, int max_x, int max_y, int ns, camera** cam, hittable** d_world, default_random_engine& e)
{
    vec3 background(0, 0, 0);
    for (int i = 0; i < max_x; ++i) {
        for (int j = 0; j < max_y; ++j) {
            vec3 col = vec3(0, 0, 0);
            int pixel_index = (j * max_x) + i;
            for (int k = 0; k < ns; ++k) {
                /*float u = float(i + random_float(e)) / float(max_x);
                float v = float(j + random_float(e)) / float(max_y);*/
                float u = float(i + 0.5f) / float(max_x);
                float v = float(j + 0.5f) / float(max_y);
                ray r = (*cam)->get_ray(u, v, e);
                vec3 temp = ray_color(r, background, d_world, e);
                col += temp;
            } //k
            //cerr << col << endl;
            col /= float(ns);
            fb[pixel_index] = col;
        } //j
    } //i
}

void creat_world(hittable** d_list, hittable** d_world, camera** cam, int nx, int ny, int num_shape, default_random_engine& e)
{
    //material *attenu_dotfive = new lambertian(vec3(0.5f, 0.5f, 0.5f));
    material *attenu_test = new lambertian(vec3(0.0, 0.5, 1.0));
    material *attenu_test2 = new lambertian(vec3(0.5, 0.5, 0.5));

    d_list[0] = new xy_rect(attenu_test, -10000, 10000, -10000, 10000, -1, 1000);
    d_list[1] = new flip_face(new xy_rect(attenu_test2, -10000, 10000, -10000, 10000, 1, 500));

    *d_world = new hittable_list(d_list, num_shape);

    vec3 lookfrom(0, 0, 0);
    vec3 lookat(0, 0, -1);
    float dist_to_focus = 1;
    float aperature = 0.0;
    float fovy = 90;    // angle
    *cam = new camera(lookfrom,
                     lookat,
                     vec3(0, 1, 0),
                     fovy,
                     float(nx) / float(ny),
                     aperature,
                     dist_to_focus,
                     0.0f,
                     1.0f);
}

/*********************************************************************************************************************/
/*********************************************************************************************************************/
/*                                code below is to compare simulation with theory                                    */
/*********************************************************************************************************************/
/*********************************************************************************************************************/
void creat_world_specify_temperature(float T1, float T2, float r1, float r2, hittable** d_list, hittable** d_world, 
    camera** cam, int nx, int ny, default_random_engine& e)
{
    int num_hittable = 2;
    //material *attenu_dotfive = new lambertian(vec3(0.5f, 0.5f, 0.5f));
    material *ref1 = new lambertian(vec3(r1, r1, r1));
    material *ref2 = new lambertian(vec3(r2, r2, r2));

    d_list[0] = new xy_rect(ref1, -10000, 10000, -10000, 10000, -1, T1);
    d_list[1] = new flip_face(new xy_rect(ref2, -10000, 10000, -10000, 10000, 1, T2));

    *d_world = new hittable_list(d_list, num_hittable);

    vec3 lookfrom(0, 0, 0);
    vec3 lookat(0, 0, -1);
    float dist_to_focus = 1;
    float aperature = 0.0;
    float fovy = 90;    // angle
    *cam = new camera(lookfrom,
                     lookat,
                     vec3(0, 1, 0),
                     fovy,
                     float(nx) / float(ny),
                     aperature,
                     dist_to_focus,
                     0.0f,
                     1.0f);
}

float calculate_theory(float T1, float T2, float r1, float r2) {
    float alpha1 = 1 - r1, alpha2 = 1 - r2;
    if (alpha1 <= 0.000001f && alpha2 <= 0.000001f) {
        return 0.0f;
    }
    else if (alpha1 <= 0.000001f) {
        return SIGMA * pow(T2, 4) / MY_PI;
    }
    else if (alpha2 <= 0.000001f) {
        return SIGMA * pow(T1, 4) / MY_PI;
    }

    // q 放热为正，吸热为负
    float q = SIGMA / (1 / alpha1 + 1 / alpha2 - 1) * (pow(T1, 4) - pow(T2, 4));

    float J1 = SIGMA * pow(T1, 4) - (1 / alpha1 - 1) * q;
    return J1 / MY_PI;
}

void compare_with_theory(float T1, float T2) {
    default_random_engine e2(time(0));
    hittable **list = new hittable*[2];
    hittable **d_world = new hittable*;
    camera **cam = new camera*;

    string outPath1a = "./Fone" + to_string(T1) + "_Ftwo" + to_string(T2) + "look_at_Fone_AND_change_Fone_ref.dat";
    string outPath1b = "./Fone" + to_string(T1) + "_Ftwo" + to_string(T2) + "look_at_Fone_AND_change_Ftwo_ref.dat";

    string outPath2a = "./Fone" + to_string(T1) + "_Ftwo" + to_string(T2) + "look_at_Ftwo_AND_change_Fone_ref.dat";
    string outPath2b = "./Fone" + to_string(T1) + "_Ftwo" + to_string(T2) + "look_at_Ftwo_AND_change_Ftwo_ref.dat";

    ofstream out1a(outPath1a), out1b(outPath1b), out2a(outPath2a), out2b(outPath2b);
    if (!out1a || !out1b || !out2a || !out2b) {
        cerr << "file open wrong!" << endl;
        exit(1);
    }

    out1a << "flat1 :" << T1 << "K flat2: " << T2 << "K" << endl;
    out1a << "look at flat1 and change fone reflectivity" << endl;
    out1b << "flat1 :" << T1 << "K flat2: " << T2 << "K" << endl;
    out1b << "look at flat1 and change ftwo reflectivity" << endl;

    out2a << "flat1 :" << T1 << "k flat2: " << T2 << "K" << endl;
    out2a << "look at flat2 and change fone reflectivity" << endl;
    out2b << "flat1 :" << T1 << "K flat2: " << T2 << "K" << endl;
    out2b << "look at flat2 and change ftwo reflectivity" << endl;

    int nx = 10,
        ny = 10;
    const int numPixels = nx * ny;

    vec3 *result_arr = new vec3[numPixels];
    float theory_radiance = 0;

    /* look f1 change f1 */
    out1a << "r1\t"
          << "r2\t"
          << "simulation\t"
          << "theory"
         << endl;
    for (float r = 0.0; r <= 1.01; r += 0.1) {
        creat_world_specify_temperature(T1, T2, r, 0.5f, list, d_world, cam, nx, ny, e2);
        render(result_arr, nx, ny, numPixels, cam, d_world, e2);
        theory_radiance = calculate_theory(T1, T2, r, 0.5f);
        out1a << r << "\t" << 0.5 << "\t" << result_arr[0][0] << "\t" << theory_radiance << endl;
        delete *d_world;
        delete *cam;
        list = new hittable*[2];
    }
    out1a.close();

    /* look f1 change f2 */
    out1b << "r1\t"
          << "r2\t"
          << "simulation\t"
          << "theory"
          << endl;
    for (float r = 0.0; r <= 1.01; r += 0.1) {
        creat_world_specify_temperature(T1, T2, 0.5, r, list, d_world, cam, nx, ny, e2);
        render(result_arr, nx, ny, numPixels, cam, d_world, e2);
        theory_radiance = calculate_theory(T1, T2, 0.5, r);
        out1b << 0.5 << "\t" << r << "\t" << result_arr[0][0] << "\t" << theory_radiance << endl;
        delete *d_world;
        delete *cam;
        list = new hittable*[2];
    }
    out1b.close();

    /* look f2 change f1 */
    out2a << "r1\t"
          << "r2\t"
          << "simulation\t"
          << "theory"
          << endl;
    for (float r = 0.0; r <= 1.01; r += 0.1) {
        creat_world_specify_temperature(T2, T1, 0.5, r, list, d_world, cam, nx, ny, e2);
        render(result_arr, nx, ny, numPixels, cam, d_world, e2);
        theory_radiance = calculate_theory(T2, T1, 0.5, r);
        out2a << r << "\t" << 0.5 << "\t" << result_arr[0][0] << "\t" << theory_radiance << endl;
        delete *d_world;
        delete *cam;
        list = new hittable*[2];
    }
    out2a.close();

    /* look f2 change f2 */
    out2b << "r1\t"
          << "r2\t"
          << "simulation\t"
          << "theory"
          << endl;
    for (float r = 0.0; r <= 1.01; r += 0.1) {
        creat_world_specify_temperature(T2, T1, r, 0.5f, list, d_world, cam, nx, ny, e2);
        render(result_arr, nx, ny, numPixels, cam, d_world, e2);
        theory_radiance = calculate_theory(T2, T1, r, 0.5f);
        out2b << 0.5 << "\t" << r << "\t" << result_arr[0][0] << "\t" << theory_radiance << endl;
        delete *d_world;
        delete *cam;
        list = new hittable*[2];
    }
    out2b.close();
    delete d_world;
    delete cam;
    delete list;
    delete[] result_arr;
}
/***************************************************************************************************************/
/***************************************************************************************************************/
/*                            code above is to compare simulation with theory                                  */
/***************************************************************************************************************/
/***************************************************************************************************************/

void free_world(hittable** d_world, camera** cam, vec3* fb) 
{
    delete *d_world;
    delete d_world;
    delete *cam;
    delete cam;
    delete[] fb;
}

int main() 
{
    default_random_engine e(time(0));

    int nx = 10, ny = 10, ns = 10;

    // color array
    const int numPixels = nx * ny;
    vec3 *fb = new vec3[numPixels];

    // initialize d_world and cam.
    const int num_hittable = 2;
    hittable **list = new hittable*[num_hittable];
    hittable **d_world = new hittable*;
    camera **cam = new camera*;

    creat_world(list, d_world, cam, nx, ny, num_hittable, e);

    // render
    clock_t start, stop;
    start = clock();
    render(fb, nx, ny, ns, cam, d_world, e);
    stop = clock();
    double seconds = ((double)(stop - start)) / CLOCKS_PER_SEC;
    cerr << "took " << seconds << " seconds of to calculate.\n" << endl;

    // write output
    cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >= 0; --j) {
        cerr << "\ralready writing: " << (int)(((double)(ny - j) / ny) * 100) << "%" << flush;
        for (int i = 0; i < nx; ++i) {
            size_t pixel_index = j * nx + i;

            cout << fb[pixel_index].x() << " " << fb[pixel_index].y() << " " << fb[pixel_index].z() << "\n";
        }
    }
    cerr << endl;
    cerr << "compare simulation with theory" << endl;
    compare_with_theory(1000, 500);

    // clean
    free_world(d_world, cam, fb);   // list is deleted while deconstruct d_world

    return 0;
}
