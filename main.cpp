#include "Raytrace.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include <iostream>
using namespace std;
color ray_color(const ray& r, const hittable& world,int depth) {//the color of ray direction's screen pixel
    if(depth<=0){
        return color(0.0, 0.0, 0.0);
    }
    hit_record rec;
    if(world.hit(r,0.001,infinity,rec)){
        //return 0.5*color(rec.normal + color(1,1,1));//map normal to color field
        point3 target = rec.p + rec.normal + random_unit_vector();
        //point3 target = rec.p + random_in_hemisphere(rec.normal);
        //center: rec.p + rec.normal     target is a random point in the sphere
        return 0.5*color(ray_color(ray(rec.p,target- rec.p),world,depth-1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);//map y to 0~1
    return t*color(0.5, 0.7, 1.0) + (1.0-t)*color(1.0, 1.0, 1.0);
}
int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int sample_per_pixel = 100;
    const int max_depth = 50;
    //World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1),0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1),100));
    //camera
    camera cam;
    // Render
    cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    for (int j = image_height-1; j >= 0; --j) {
        cerr << "\rcurrent: " <<100.f*(image_height-j)/(image_height)<<"%"<< flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0,0,0);
            for (int s = 0; s < sample_per_pixel; s++)
            {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u,v);
                pixel_color += ray_color(r,world,max_depth);
            }
            write_color(cout, pixel_color,sample_per_pixel);
        }
    }
    cerr << "\nDone.\n";
}