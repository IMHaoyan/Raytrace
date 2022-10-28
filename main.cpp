#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "sphere.h"
#include "hittalbe.h"
#include <iostream>
using namespace std;
double hit_sphere(const point3& center,double radius,const ray& r){
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(),r.direction());//length_square
    auto half_b = dot(r.direction(),oc);
    auto c = dot(oc,oc) - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    if(discriminant>0){
        return (-half_b - sqrt(discriminant)) / a;
    }else{
        return -1.0;
    }
}
color ray_color(const ray& r) {//the color of ray direction's screen pixel
    sphere sphere(point3(0,0,-1),0.5);
    auto t = hit_sphere(point3(0,0,-1),0.5,r);
    hit_record hit_rec;
    if(sphere.hit(r,0,10,hit_rec)){
        vec3 N = hit_rec.normal;
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }
    
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5*(unit_direction.y() + 1.0);//map y to 0~1
    return t*color(0.5, 0.7, 1.0) + (1.0-t)*color(1.0, 1.0, 1.0);
}
int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    // Camera

    auto viewport_height = 2.0;//the height of view port is const 2
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
    //the "viewport_width x viewport_height x 1?" cube's lower_left_corner

    // Render
    cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    for (int j = image_height-1; j >= 0; --j) {
       // cerr << "\rScanlines remaining: " << j << ' ' << flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color(cout, pixel_color);
        }
    }
    cerr << "\nDone.\n";
}