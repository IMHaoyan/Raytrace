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
//material.h
class dielectric : public material {
    public:
        dielectric(double ri) : ref_idx(ri) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
        ) const {
            attenuation = vec3(1.0, 1.0, 1.0);
            double etai_over_etat;
            if (rec.front_face) {
                etai_over_etat = 1.0 / ref_idx;
            } else {
                etai_over_etat = ref_idx;
            }

            vec3 unit_direction = unit_vector(r_in.direction());
            vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
            scattered = ray(rec.p, refracted);
            return true;
        }

        double ref_idx;
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