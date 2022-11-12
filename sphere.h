#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"
#include "onb.h"
#include "vec3.h"

class sphere : public hittable {
public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
public:
    sphere() {}
    sphere(point3 cen, double r, shared_ptr<material> m)
        : center(cen), radius(r), mat_ptr(m){};
    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const override;
    virtual bool bounding_box(aabb& output_box) const override;

    double pdf_value(const point3& o, const vec3& v) const {
        hit_record rec;
        if (!this->hit(ray(o, v), 0.001, infinity, rec)) return 0;

        auto cos_theta_max = sqrt(1 - radius * radius / (center - o).length_squared());
        auto solid_angle = 2 * pi * (1 - cos_theta_max);
        return 1 / solid_angle;
    }

    vec3 random(const point3& o) const {
        vec3 direction = center - o;
        auto distance_squared = direction.length_squared();
        onb uvw;
        uvw.build_from_w(direction);
        return uvw.local(random_to_sphere(radius, distance_squared));
    }
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());  // length_square
    auto half_b = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);
    double t = (-half_b - sqrtd) / a;
    if (t < t_min || t > t_max) {
        t = (-half_b + sqrtd) / a;
        if (t < t_min || t > t_max) return false;
    }
    rec.t = t;
    rec.p = r.at(t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    return true;
}

bool sphere::bounding_box(aabb& output_box) const {
    output_box = aabb(center - vec3(radius, radius, radius),
                      center + vec3(radius, radius, radius));
    return true;
}

#endif