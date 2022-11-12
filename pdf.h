#ifndef PDF_H
#define PDF_H
#include "onb.h"
#include "vec3.h"
#include "hittable_list.h"
class pdf {
public:
    virtual ~pdf() {}

    virtual double value(const vec3& direction) const = 0;
    virtual vec3 generate() const = 0;
};

class cosine_pdf : public pdf {
public:
    onb uvw;

public:
    cosine_pdf(const vec3& w) { uvw.build_from_w(w); }

    virtual double value(const vec3& direction) const override {
        auto cosine = dot(unit_vector(direction), uvw.w());
        return cosine > 0 ? cosine / pi : 0;
    }

    virtual vec3 generate() const override {
        return uvw.local(random_cosine_direction());
    }
};

class hittable_pdf : public pdf {
public:
    point3 o;
    shared_ptr<hittable> ptr;

public:
    hittable_pdf(shared_ptr<hittable> p, const point3& origin) : 
        ptr(p), o(origin) {}
    
    virtual double value(const vec3& direction) const override {
        return ptr->pdf_value(o, direction);
    }
    virtual vec3 generate() const override {
        return ptr->random(o);
    }
};
class hittablelist_pdf : public pdf {
public:
    point3 o;
    hittable_list ptr;

public:
    hittablelist_pdf(hittable_list p, const point3& origin) : 
        ptr(p), o(origin) {}
    
    virtual double value(const vec3& direction) const override {
        return ptr.pdf_value(o, direction);
    }
    virtual vec3 generate() const override {
        return ptr.random(o);
    }
};

class mix_pdf : public pdf{
public:
    shared_ptr<pdf> pdfs[2];
public:
    mix_pdf(shared_ptr<pdf> object_pdf, shared_ptr<pdf> light_pdf){
        pdfs[0] = object_pdf;
        pdfs[1] = light_pdf;
    }
    virtual double value(const vec3& direction) const override {
        return 0.5 * pdfs[0]->value(direction) + 0.5* pdfs[1]->value(direction);
    }
    virtual vec3 generate() const override {
        if(random_double() > 0.5){
            return pdfs[0]->generate();
        }else{
            return pdfs[1]->generate();
        }
    }
};
#endif