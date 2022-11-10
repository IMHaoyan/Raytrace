#ifndef MATERIAL_H
#define MATERIAL_H
#include "Raytrace.h"
#include "hittable.h"
#include "texture.h"
#include "onb.h"
struct hit_record;
// material可以由入射处信息确定 衰减率，反射方向
class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& albedo,
                         ray& scattered, double& pdf) const {
        return false;
    }
    virtual double scattering_pdf(const ray& r_in, const hit_record& rec,
                                  const ray& scattered) const {
        return 0.0;
    }
    virtual color emitted(const ray& r_in, const hit_record& rec, double u, double v,
        const point3& p) const {
        return color(0, 0, 0);
    }
};
class diffuse_light : public material {
public:
    shared_ptr<texture> emit;
public:
    diffuse_light() {}
    diffuse_light(shared_ptr<texture> _emit) : emit(_emit) {}
    diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& albedo,
                         ray& scattered, double& pdf) const {
        return false;
    }
    color emitted(const ray& r_in, const hit_record& rec, double u, double v,
        const point3& p) const {
        if(rec.front_face){
            return emit->value(u, v, p);
        }else{
            return color(0,0,0);
        }
    }
};
class lambertian : public material {
public:
    shared_ptr<texture> albedo;

public:
    lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& alb,
                         ray& scattered, double& pdf) const {
        onb uvw;
        uvw.build_from_w(rec.normal);
        auto direction = uvw.local(random_cosine_direction());
        scattered = ray(rec.p, unit_vector(direction));
        alb = albedo->value(rec.u, rec.v, rec.p);
        pdf = dot(uvw.w(), scattered.direction()) / pi;//此时见random_cosine_direction，生成函数pdf为cos/pi
        return true;
    }
    double scattering_pdf(const ray& r_in, const hit_record& rec,
                          const ray& scattered) const {
        auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
        return cosine > 0 ? cosine / pi : 0;
    }
};
// class diffuse : public material{
//     public:
//         shared_ptr<texture> albedo;
//     public:
//         diffuse(const color& a): albedo(make_shared<solid_color>(a)){}
//         diffuse(shared_ptr<texture> a) : albedo(a){}

//         virtual bool scatter(const ray& r_in, const hit_record& rec,color& attenuation,
//             ray& scattered) const override{
//             auto scatter_direction = rec.normal + random_unit_vector();
//             if(scatter_direction.near_zero()){
//                 scatter_direction = rec.normal;
//             }
//             scattered = ray(rec.p, scatter_direction);
//             attenuation = albedo->value(rec.u, rec.v, rec.p)/pi;
//             return true;
//         }
//         virtual float pdf() const override{
//             return 0.5/pi;
//         }
// };
// class metal : public material{
//     public:
//         color albedo;
//         double fuzz;
//     public:
//         metal(const color& a) : albedo(a) {fuzz=0;}
//         metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
//         virtual bool scatter(const ray& r_in, const hit_record& rec,color& attenuation,
//             ray& scattered) const override{
//             auto reflected = reflect(unit_vector(r_in.direction()), rec.normal);
//             scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
//             attenuation = albedo;   //衰减率attenuation
//             return dot(scattered.direction(),rec.normal)>0;
//         }
// };

// class dielectric : public material {
//     public:
//         double ir;
//     public:
//         dielectric(double index_of_refraction) : ir(index_of_refraction) {}
//         virtual bool scatter(const ray& r_in, const hit_record& rec, color&
//         attenuation,
//             ray& scattered) const override {
//             attenuation = color(1.0, 1.0, 1.0);
//             double refraction_ratio = rec.front_face ? (1.0/ir) : ir;
//             vec3 unit_direction = unit_vector(r_in.direction());
//             double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
//             double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
//             bool cannot_refract = refraction_ratio * sin_theta > 1.0;
//             vec3 direction;

//             if (cannot_refract||reflectance(cos_theta, refraction_ratio) >
//             random_double()){
//                 direction = reflect(unit_direction, rec.normal);
//             }//光路不是可逆吗 为什么对于球体 可以有cannot_refract的情况
//             else
//                 direction = refract(unit_direction, rec.normal, refraction_ratio);
//             scattered = ray(rec.p, direction);
//             return true;
//         }
//     private:
//         static double reflectance(double cosine, double ref_idx) {
//             // Use Schlick's approximation for reflectance.
//             auto r0 = (1-ref_idx) / (1+ref_idx);
//             r0 = r0*r0;
//             return r0 + (1-r0)*pow((1 - cosine),5);
//         }
// };

#endif