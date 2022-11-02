#include "Raytrace.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include <iostream>
#include "material.h"
#include "bvh.h"
#include <ctime>
#include<iomanip>
using namespace std;

hittable_list random_scene() {
    hittable_list world;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    //return world;
    return hittable_list(make_shared<bvh_node>(world));//动态模糊请加time0，time1的实参 参考33行
}

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
            //return 0.5*color(unit_vector(scattered.direction()) + color(1,1,1));
            return attenuation * ray_color(scattered, world, depth-1);
        }else{
            return color(0,0,0);//absorbed
        }
    }
    else{
        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.y() + 1.0);//map y to 0~1
        return t*color(0.5, 0.7, 1.0) + (1.0-t)*color(1.0, 1.0, 1.0);
    }
}

int main() {
    // Image
    auto aspect_ratio = 3.0 / 2.0;
    int image_width = 200;
    int samples_per_pixel = 50;
    const int max_depth = 50;

    // World
    auto world = random_scene();
    
    // Camera
    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    // Render
    cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    
    clock_t begin = clock(), end;
    for (int j = image_height-1; j >= 0; --j) {
        end = clock();
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0,0,0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u,v);
                pixel_color += ray_color(r,world,max_depth);
                cerr << "\rcurrent: " <<fixed<<setprecision(2)<<100.0*(image_height-j)/(image_height)<<"%   "
                    <<"Time past: "<<fixed<<setprecision(3)<<double(end-begin)/1000000<<" s"<<flush;
            }
            write_color(cout, pixel_color,samples_per_pixel);
        }
    }
    cerr << "\nDone.\n\n";
    return 0;
}