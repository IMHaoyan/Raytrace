#ifndef AABB_H
#define AABB_H
#include "Raytrace.h"
//层次包围盒  三个面围成
class aabb{

    public:
        aabb(){}
        aabb(const point3& a,const point3& b):minimum(a),maximum(b){} //a b为一对面的系数 一共三对

        point3 min(){return minimum;}
        point3 max(){return maximum;}

        bool hit(const ray& r, double t_min, double t_max) const{
            for (int i = 0; i < 3; i++) {
                auto invD = 1.0f / r.direction()[i];
                auto t0 = (minimum[i] - r.origin()[i]) * invD;
                auto t1 = (maximum[i] - r.origin()[i]) * invD;
                if (invD < 0.0f)
                    swap(t0, t1);
                t_min = t0 > t_min ? t0 : t_min;
                t_max = t1 < t_max ? t1 : t_max;
                if (t_max <= t_min)
                    return false;
            }
            return true;
        }

    public:
        point3 minimum;
        point3 maximum;  
};
aabb surrounding_box(aabb box0, aabb box1) {
    point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));

    point3 big(fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));

    return aabb(small,big);
}
#endif