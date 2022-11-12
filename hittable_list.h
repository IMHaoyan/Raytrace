#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "hittable.h"
using namespace std;

class hittable_list : public hittable {
public:
    vector<shared_ptr<hittable>> objects;

public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); };

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }
    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const override;
    virtual bool bounding_box(aabb& output_box) const override;

    double pdf_value(const point3& o, const vec3& v) const {
        auto weight = 1.0 / objects.size();
        auto sum = 0.0;

        for (const auto& object : objects) sum += weight * object->pdf_value(o, v);

        return sum;
    }

    vec3 random(const vec3& o) const {
        auto int_size = static_cast<int>(objects.size());
        return objects[random_int(0, int_size - 1)]->random(o);
    }
};

bool hittable_list ::bounding_box(aabb& output_box) const {
    if (objects.empty()) return false;
    aabb temp_box;
    // my idea： init: output_box = aabb(point3(0,0,0),point3(0,0,0));
    bool first_box = true;
    for (const auto& object : objects) {
        if (!object->bounding_box(temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }
    return true;
}
bool hittable_list ::hit(const ray& r, double t_min, double t_max,
                         hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;
    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

#endif