#ifndef BOX_H
#define BOX_H
#include "aarect.h"
#include "hittable_list.h"
class box : public hittable {
public:
    point3 box_min,box_max;
    hittable_list sides;
public:
    box(){}
    box(const point3& p0, const point3& p1, shared_ptr<material> mat)
        : box_min(p0), box_max(p1){
        sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat));
        sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat));
        sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat));
        sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat));
        sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat));
        sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat));
    }
    virtual bool hit(const ray& r,double t_min,double t_max, hit_record& rec) const override{
        return sides.hit(r, t_min, t_max, rec);
    }
    virtual bool bounding_box(aabb& output_box) const override {
            output_box = aabb(box_min, box_max);
            return true;
    }
};

#endif