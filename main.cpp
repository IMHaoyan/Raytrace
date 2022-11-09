#include "head.h"
using namespace std;
using namespace std::chrono;
hittable_list random_scene();
hittable_list simple_light_scene();
hittable_list cornell_box();

int image_height = 300;
int samples_per_pixel = 10;
auto aspect_ratio = 1.0;

const int max_depth = 5;
const int num_threads = 10;
color background = color(0,0,0);
// World

hittable_list world = cornell_box();

auto aperture = 0.0;
auto lookfrom = point3(278, 278, 800);
auto lookat = point3(278, 278, 0);
auto vfov = 40.0;

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

    auto MatLight = make_shared<diffuse_light>(color(1, 1, 1));
    world.add(make_shared<sphere>(point3(0, 1.3, 0), 1.3, MatLight));

    auto MatDiffuse = make_shared<diffuse>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-3.5, 1.3, 0), 1.3, MatDiffuse));

    auto MatLambertian = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(3, 1.3, 0), 1.3, MatLambertian));

    return hittable_list(make_shared<bvh_node>(world));
}

hittable_list simple_light_scene() {
    hittable_list objects;

    auto Diffuse = make_shared<diffuse>(color(1,1,1));
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, Diffuse));
    objects.add(make_shared<sphere>(point3(0,2,0), 2, Diffuse));

    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

    return objects;
}

hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, -555, 0, 0, green));
    objects.add(make_shared<yz_rect>(0, 555, -555, 0, 555, red));
    objects.add(make_shared<xz_rect>(213, 343, -332, -227, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, -555, 0, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, -555, 0, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, -555, white));

    auto Metal = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    auto Diffuse = make_shared<diffuse>(color(1.00, 0.71, 0.29));

    //objects.add(make_shared<sphere>(point3(222,150,-333), 150, Metal));
    // objects.add(make_shared<sphere>(point3(444,70,-222), 70, Diffuse));
    
    objects.add(make_shared<box>(point3(260, 0, -230), point3(425, 165, -65), white));
    objects.add(make_shared<box>(point3(125, 0, -460), point3(290,330,-295), white));
    return objects;
}

color ray_color(const ray &r, const color &background, const hittable &world,
                int depth) {
    double RR = 1.0;
    if (depth <= 0) {
        RR = 0.8;
    }
    if (random_double() > RR) {
        return color(0, 0, 0);
    }
    hit_record rec;
    if (!world.hit(r, 0.001, infinity, rec)) {
        return background;
    }
    ray scattered;
    color attenuation;
    color emit = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
        return emit;
    }else{
        return emit + attenuation * ray_color(scattered, background, world, depth - 1) 
                * dot(unit_vector(scattered.direction()), unit_vector(rec.normal)) 
                / rec.mat_ptr->pdf() / RR;
    }
    // vec3 unit_direction = unit_vector(r.direction());
    // auto t = 0.5 * (unit_direction.y() + 1.0);
    // return t * color(0.5, 0.7, 1.0) + (1.0 - t) * color(1.0, 1.0, 1.0);
}

int main() {
    // Camera
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;

    auto aperture = 0.0;
    int image_width = static_cast<int>(image_height * aspect_ratio);
    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture,
               dist_to_focus);
    // Render
    cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    vector<color> framebuffer(image_height * image_width);
    mutex mtx;
    int progress = 0;
    thread th[num_threads];
    int thread_height = image_height / num_threads;

    auto begin = system_clock::now(), end = begin;
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
                        ray_color(r, background, world, max_depth) / samples_per_pixel;
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