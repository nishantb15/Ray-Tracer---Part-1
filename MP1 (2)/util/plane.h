#ifndef PLANE_H
#define PLANE_H

#include "hittable.h"
#include "vec3.h"

class plane : public hittable {
    public:
        plane() : kd(vec3(0,0,0)), ld(vec3(0,0,0)) {}
        plane(vec3 pu, vec3 nu, vec3 kdu, vec3 ldu) : p(pu), n(nu), kd(kdu), ld(ldu) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        vec3 p;
		vec3 n;
		vec3 kd;
		vec3 ld;
};

#endif
