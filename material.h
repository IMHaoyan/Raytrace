#ifndef MATERIAL_H
#define MATERIAL_H
#include "Raytrace.h"
#include "hittable.h"
#include "texture.h"
#include "onb.h"
#include "pdf.h"
struct hit_record;

struct scatter_record{
    ray specular_ray;
    bool is_specular;
    color attenuation;
    shared_ptr<pdf> pdf_ptr;
};

class material {
public:
    int id;
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const {
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
    diffuse_light() {id =1;}
    diffuse_light(shared_ptr<texture> _emit) : emit(_emit) {id =1;}
    diffuse_light(color c) : emit(make_shared<solid_color>(c)) {id =1;}

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
    lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {id =2;}
    lambertian(shared_ptr<texture> a) : albedo(a) {id =2;}

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const {
        srec.is_specular = false;
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
        return true;
    }
    double scattering_pdf(const ray& r_in, const hit_record& rec,
                          const ray& scattered) const {
        auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
        return cosine > 0 ? cosine / pi : 0;
    }
};

class metal : public material {
    public:
        color albedo;
        double fuzz;
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {id =3;}
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, scatter_record& srec
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            srec.specular_ray = ray(rec.p, reflected+fuzz*random_in_unit_sphere());
            srec.attenuation = albedo;
            srec.is_specular = true;
            srec.pdf_ptr = 0;
            return true;
        }
};

class dielectric : public material {
    public:
        double ir;
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const override {
            srec.attenuation = color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;
            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract||reflectance(cos_theta, refraction_ratio) >
            random_double()){
                direction = reflect(unit_direction, rec.normal);
            }//光路不是可逆吗 为什么对于球体 可以有cannot_refract的情况
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            srec.specular_ray = ray(rec.p, direction);
            srec.pdf_ptr = 0;
            srec.is_specular = true;
            return true;
        }
    private:
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};

#endif