#ifndef SPHERE_H
#define SPHERE_H
#include "hittalbe.h"
#include "vec3.h"
class sphere : public hittalbe{
    public:
        sphere(){}
        sphere(point3 cen,double r):center(cen),radius(r){}
        virtual bool hit(const ray& r,double t_min,double t_max,
            hit_record& rec) const override;
    public:
        point3 center;
        double radius;
};
bool sphere::hit(const ray& r,double t_min,double t_max,
            hit_record& rec) const{
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(),r.direction());//length_square
    auto half_b = dot(r.direction(),oc);
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    if(discriminant<0)
        return false;
    auto sqrtd = sqrt(discriminant);
    double t = (-half_b - sqrtd) / a;
    if(t<t_min||t>t_max){
        t = (-half_b + sqrtd) / a;
        if(t<t_min||t>t_max)
            return false;
    }
    rec.t = t;
    rec.p = r.at(t);
    vec3 outward_normal = (rec.p - center)/radius;
    rec.set_face_normal(r,outward_normal);

    return true;
}
#endif