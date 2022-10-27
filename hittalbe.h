#ifndef HITTABLE_H
#define HITTABLE_H
#include "vec3.h"
#include "ray.h"
struct hit_record
{
    vec3 p;
    vec3 normal;
    double t;
};
class hittalbe
{
   public:
        virtual bool hit(const ray& r,double t_min,double t_max,
            hit_record& rec) const = 0;
};

#endif