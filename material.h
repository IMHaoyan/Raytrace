#ifndef MATERIAL_H
#define MATERIAL_H
#include "Raytrace.h"
#include "hittable.h"
#include "onb.h"
#include "pdf.h"
#include "texture.h"
struct hit_record;
using namespace std;
struct scatter_record {
    ray specular_ray;
    bool is_specular;
    color attenuation;
    shared_ptr<pdf> pdf_ptr;
};

class material {
public:
    int id;
    
public:
    virtual color micro_attention(const vec3& wi, const vec3& wo, const vec3& N) const {
        return color(1, 0, 0);
    }
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         scatter_record& srec) const {
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
    diffuse_light() { id = 1; }
    diffuse_light(shared_ptr<texture> _emit) : emit(_emit) { id = 1; }
    diffuse_light(color c) : emit(make_shared<solid_color>(c)) { id = 1; }

    bool scatter(const ray& r_in, const hit_record& rec, color& albedo, ray& scattered,
                 double& pdf) const {
        return false;
    }
    color emitted(const ray& r_in, const hit_record& rec, double u, double v,
                  const point3& p) const {
        if (rec.front_face) {
            return emit->value(u, v, p);
        } else {
            return color(0, 0, 0);
        }
    }
};
class lambertian : public material {
public:
    shared_ptr<texture> albedo;

public:
    lambertian(const color& a) : albedo(make_shared<solid_color>(a)) { id = 2; }
    lambertian(shared_ptr<texture> a) : albedo(a) { id = 2; }

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
float fresnel(const vec3& I,
                 const vec3& N,
                 const float& ior) {
        float cosi = clamp(-1, 1, dot(I, N));
        float etai = 1, etat = ior;
        if (cosi > 0) {
            swap(etai, etat);
        }
        // Compute sini using Snell's law
        float sint = etai / etat * sqrtf(max(0.f, 1 - cosi * cosi));
        // Total internal reflection
        if (sint >= 1) {
            return 1.0;
        } else {
            float cost = sqrtf(max(0.f, 1 - sint * sint));
            cosi = fabsf(cosi);
            float Rs = ((etat * cosi) - (etai * cost)) /
                       ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) /
                       ((etai * cosi) + (etat * cost));
            return (Rs * Rs + Rp * Rp) / 2;
        }
        // As a consequence of the conservation of energy, transmittance is
        // given by: kt = 1 - kr;
    }
    float DistributionGGX(vec3 N, vec3 H, float roughness) {
        float a = roughness * roughness;
        float a2 = a * a;
        float NdotH = dot(N, H) > 0.0f ? dot(N, H) : 0.0f;
        float NdotH2 = NdotH * NdotH;

        float nom = a2;
        float denom = (NdotH2 * (a2 - 1.0) + 1.0);
        denom = M_PI * denom * denom;

        return nom / max(denom, 0.0000001f);
        // prevent divide by zero for
        // roughness=0.0 and NdotH=1.0
    }

    float SchlickGGX(float NdotV, float roughness) {
        float r = (roughness + 1.0);
        float k = (r * r) / 8.0;

        float nom = NdotV;
        float denom = NdotV * (1.0 - k) + k;

        return nom / denom;
    }

    float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
        float NdotV = dot(N, V) > 0.0f ? dot(N, V) : 0.0f;
        float NdotL = dot(N, L) > 0.0f ? dot(N, L) : 0.0f;
        float ggx2 = SchlickGGX(NdotV, roughness);
        float ggx1 = SchlickGGX(NdotL, roughness);

        return ggx1 * ggx2;
    }
class Mro_lambertian : public material {
public:
    shared_ptr<texture> albedo;
    double ior;
    color Kd;
    double rough;
    double metallic = 0.0;

public:
    Mro_lambertian(const color& a) : albedo(make_shared<solid_color>(a)) { id = 9; }
    Mro_lambertian(shared_ptr<texture> a) : albedo(a) { id = 9; }

    double scattering_pdf(const ray& r_in, const hit_record& rec,
                          const ray& scattered) const {
        auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
        return cosine > 0 ? cosine / pi : 0;
    }
    

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const {
        srec.is_specular = false;
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
        return true;
    }

    vec3 micro_attention(const vec3& wi, const vec3& wo, const vec3& N) const override{
        //return vec3(1,1,1);
        float cosalpha = dot(N, wo);
            if (cosalpha > 0.0f) {
                float roughness = 0.0f, etat =1.85;
                roughness = this->rough;
                vec3 V = -wi;
                vec3 L = wo;
                vec3 H = unit_vector(V + L);
                float D = DistributionGGX(N, H, roughness);
                float G = GeometrySmith(N, V, L, roughness);
                float F= fresnel(wi, N, etat);
                float per = D * G * F;
                vec3 nominator = vec3(per, per, per);
                float denominator = 4 * max(dot(N, V), 0.0f) *
                                    max(dot(N, L), 0.0f);
                vec3 specular = nominator / max(denominator, 0.001f);

                // 能量守恒
                float ks_ = F;
                float kd_ = 1.0f - ks_;

                vec3 diffuse = Kd * (1.0f-metallic) / M_PI;

                // 因为在 specular里已考虑折射部分的比例：F，所以折射部分不需再乘 ks_
                // （ks_ * Ks * specular）
                //return kd_ * Kd * diffuse + Ks * specular;
                return specular* M_PI;
                return (kd_ * diffuse +  vec3(10,0,0)*specular)* M_PI;
            } else
                return vec3(0,0,0);
    }

};

class metal : public material {
public:
    color albedo;
    double fuzz;

public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) { id = 3; }
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         scatter_record& srec) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        srec.specular_ray = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        srec.attenuation = albedo;
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;
        return true;
    }
};

class dielectric : public material {
public:
    double ir;

public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) { id = 4; }
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         scatter_record& srec) const override {
        srec.attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;
        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract ||
            reflectance(cos_theta, refraction_ratio) > random_double()) {
            direction = reflect(unit_direction, rec.normal);
        }  // 光路不是可逆吗 为什么对于球体 可以有cannot_refract的情况
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
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif