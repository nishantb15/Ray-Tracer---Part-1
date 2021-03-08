#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"

class triangle : public hittable {
    public:
        triangle() : kd(vec3(0,0,0)), ld(vec3(0,0,0)) {}
        triangle(vec3 v1u, vec3 v2u, vec3 v3u, vec3 kdu, vec3 ldu) : v1(v1u), v2(v2u), v3(v3u), kd(kdu), ld(ldu) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        vec3 v1;
		vec3 v2;
		vec3 v3;
		vec3 kd;
		vec3 ld;
};

#endif
