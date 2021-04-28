#ifndef PDF_H
#define PDF_H

#include "vec3.h"
#include "onb.h"
#include "hittable.h"
#include "myUtility.h"

class pdf {
public:
    virtual ~pdf(){};

    virtual float value(const vec3 &direction) const = 0;
    virtual vec3 generate(default_random_engine& e) const = 0;
};

class cosine_pdf : public pdf {
public:
    cosine_pdf(const vec3 &w) { uvw.build_from_w(w); }

    virtual float value(const vec3 &direction) const
    {
        float cosine = dot(unit_vector(direction), uvw.w());
        return (cosine <= 0) ? 0 : cosine / MY_PI;
    }

    virtual vec3 generate(default_random_engine& e) const
    {
        return uvw.local(random_cosine_direction(e));
    }

public: 
    onb uvw;
};

#endif