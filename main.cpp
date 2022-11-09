#include "head.h"
using namespace std;
using namespace std::chrono;
hittable_list random_scene();

auto aspect_ratio = 16.0 / 9.0;
int image_height = 360;
int samples_per_pixel = 10;
const int max_depth = 5;
const int num_threads = 18;

// World
hittable_list world = random_scene();
point3 lookfrom = point3(6, 4, 12);
point3 lookat = point3(0, 1, 0);
auto vfov = 20.0;
auto aperture = 0.0;

hittable_list random_scene() {
    hittable_list world;
    auto checker = make_shared<lambertian>(make_shared<checker_texture>(
        color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9)));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, checker));

    // for (int x = -10; x <= 2; x += 2) {
    //     for (int z = -12; z <= 4; z += 2) {
    //         auto choose_mat = random_double();
    //         point3 center(x + 0.9 * random_double(), 0.2,
    //                       z + 0.9 * random_double());
    //         if ((center - point3(4, 0.2, 0)).length() > 0.9) {
    //             shared_ptr<material> sphere_material;
    //             if (choose_mat < 0.8) {
    //                 // diffuse
    //                 auto albedo = color::random() * color::random();
    //                 sphere_material = make_shared<lambertian>(albedo);
    //                 world.add(
    //                     make_shared<sphere>(center, 0.2, sphere_material));
    //             } else if (choose_mat < 0.95) {
    //                 // metal
    //                 auto albedo = color::random(0.5, 1);
    //                 auto fuzz = random_double(0, 0.5);
    //                 sphere_material = make_shared<metal>(albedo, fuzz);
    //                 world.add(
    //                     make_shared<sphere>(center, 0.2, sphere_material));
    //             } else {
    //                 // glass
    //                 sphere_material = make_shared<dielectric>(1.5);
    //                 world.add(
    //                     make_shared<sphere>(center, 0.2, sphere_material));
    //             }
    //         }
    //     }
    // }

    //auto material1 = make_shared<dielectric>(1.5);
    //world.add(make_shared<sphere>(point3(0, 1.3, 0), 1.3, material1));
    auto Diffuse = make_shared<diffuse>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-3.5, 1.3, 0), 1.3, Diffuse));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(0, 1.3, 0), 1.3, material2));
    
    return hittable_list(make_shared<bvh_node>(
        world));
}

color ray_color(const ray& r, const hittable& world, int depth) {
    double RR = 1.0;
    if (depth <= 0) {
        RR = 0.8;
    }
    if(random_double() > RR){
        return color(0, 0, 0);
    }
    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1) 
                    * dot(unit_vector(scattered.direction()) , unit_vector(rec.normal)) 
                   / rec.mat_ptr->pdf() / RR ;
        } else {
            return color(0, 0, 0);  // absorbed
        }
    } else {//background
        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return t * color(0.5, 0.7, 1.0) + (1.0 - t) * color(1.0, 1.0, 1.0);
    }
}

int main() {
    // Camera
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    int image_width = static_cast<int>(image_height * aspect_ratio);
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture,
               dist_to_focus);
    // Render
    cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    vector<color> framebuffer(image_height * image_width);
    mutex mtx;
    int progress = 0;
    thread th[num_threads];
    int thread_height = image_height / num_threads;

    auto begin = system_clock::now(),end = begin;
    duration<double> diff = end - begin;
    cerr << "\nimage_height:\t" << image_height << "\t"
         << "spp:\t" << samples_per_pixel << "\n";
    auto renderRows = [&](int start_height, int end_height) {
        for (int j = start_height; j < end_height; ++j) {
            end = system_clock::now();
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; s++) {
                    auto u = (i + random_double()) / (image_width - 1);
                    auto v = (j + random_double()) / (image_height - 1);
                    ray r = cam.get_ray(u, v);
                    pixel_color +=
                        ray_color(r, world, max_depth) / samples_per_pixel;
                }
                framebuffer[j * image_width + image_width - 1 - i] =
                    pixel_color;
            }
            mtx.lock();
            progress++;
            diff = end - begin;
            cerr << "\rcurrent: " << fixed << setprecision(2)
                 << 100.0 * (progress) / (image_height) << "%   "
                 << "Time past: " << fixed << setprecision(3) << diff.count()
                 << " s" << flush;
            mtx.unlock();
        }
    };
    for (int t = 0; t < num_threads; ++t) {
        th[t] = thread(renderRows, t * thread_height, (t + 1) * thread_height);
    }
    for (int t = 0; t < num_threads; ++t) {
        th[t].join();
    }
    int buffer_size = image_width * image_height;
    for (auto i = 0; i < buffer_size; ++i) {
        write_color(cout, framebuffer[buffer_size - 1 - i], 1.0);
    }
    cerr << "\nDone.\n";
    return 0;
}