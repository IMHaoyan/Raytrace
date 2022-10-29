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
    public:
        metal(const color& a) : albedo(a) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec,color& attenuation, 
            ray& scattered) const override{
            auto reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected);
            attenuation = albedo; //衰减率attenuation
            return dot(scattered.direction(),rec.normal)>0;
        }
};
#endif