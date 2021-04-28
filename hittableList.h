/*
 * this file define a list that contain all the hittable object.
 * Define function hit() to iteratively call the hit() function of those
 * hittable object to get the closest hitted object and record the hit point in hit_record
*/

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

class hittable_list : public hittable {
public:
    hittable_list() { };
    hittable_list(hittable **l, int n) {list = l; list_size = n;}

    ~hittable_list() {
        for (int i = 0; i < list_size; ++i) {
            delete list[i];
        }
        delete list;
    }

    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;

public:
    hittable **list;
    int list_size;
};

#endif