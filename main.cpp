#include "Raytrace.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include <iostream>
#include "material.h"
using namespace std;
// Image
const auto aspect_ratio = 16.0 / 9.0;
const int image_width = 600;
const int image_height = static_cast<int>(image_width / aspect_ratio);
const int sample_per_pixel = 10;
const int max_depth = 50;
//camera
camera cam;
//World
hittable_list world;

color ray_color(const ray& r, const hittable& world,int depth) {//the color of ray direction's screen pixel
    if(depth<=0){
        return color(0.0, 0.0, 0.0);
    }
    hit_record rec;
    if(world.hit(r,0.001,infinity,rec)){
        // return 0.5*color(rec.normal + color(1,1,1));//map normal to color field
        // point3 target = rec.p + random_in_hemisphere(rec.normal);
        // point3 target = rec.p + rec.normal + random_unit_vector();
        // return 0.5*color(ray_color(ray(rec.p,target- rec.p),world,depth-1));
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r,rec,attenuation,scattered)){
            return attenuation * ray_color(scattered, world, depth-1);
                //*abs(dot(unit_vector(rec.normal),unit_vector(scattered.direction())));
        }
        return color(0,0,0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);//map y to 0~1
    return t*color(0.5, 0.7, 1.0) + (1.0-t)*color(1.0, 1.0, 1.0);
    //return t*color(1.0, 0.7, 0.5) + (1.0-t)*color(1.0, 1.0, 1.0);
}

int main() {
    auto material_behind = make_shared<lambertian>(color(0.8, 0.6, 0.2));
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<dielectric>(1.5);
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0,  1.1),   1.0, material_behind));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

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