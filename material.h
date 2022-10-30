#ifndef MATERIAL_H
#define MATERIAL_H
#include "Raytrace.h"
#include "hittable.h"
struct hit_record;
class material{
    public:
        virtual bool scatter(const ray& r_in, const hit_record& rec,color& attenuation, 
            ray& scattered) const = 0;
};

class dielectric : public material {
    public:
        double ir;
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, 
            ray& scattered) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;
            vec3 unit_direction = unit_vector(r_in.direction());
            vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);
            scattered = ray(rec.p, refracted);
            return true;
        }
};
//material可以由入射处信息确定 衰减率，反射方向
class lambertian : public material{
    public:
        color albedo;
    public:
        lambertian(const color& a): albedo(a){}
        virtual bool scatter(const ray& r_in, const hit_record& rec,color& attenuation, 
            ray& scattered) const override{
            auto scatter_direction = rec.normal + random_unit_vector();
            if(scatter_direction.near_zero()){
                scatter_direction = rec.normal;
            }
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo; //衰减率attenuation
            return true;
        }
};
class metal : public material{
    public:
        color albedo;
        double fuzz;
    public:
        metal(const color& a) : albedo(a) {fuzz=0;}
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec,color& attenuation, 
            ray& scattered) const override{
            auto reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo; //衰减率attenuation
            return dot(scattered.direction(),rec.normal)>0;
        }
};
#endif