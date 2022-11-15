#include "head.h"
using namespace std;
using namespace std::chrono;
hittable_list random_scene();
hittable_list simple_light_scene();
hittable_list cornell_box();
hittable_list final();
int image_height = 800;
int samples_per_pixel = 128;
auto aspect_ratio = 1.0;
//make && ./Raytrace > output_cos.ppm && eog output_cos.ppm
const int bounce = 5;
const int max_depth = bounce + 1;
const int num_threads = 10;
color background = color(0, 0, 0);
hittable_list world = cornell_box();

auto lookfrom = point3(278, 278, -800);
auto lookat = point3(278, 278, 0);
vec3 vup(0, 1, 0);
auto vfov = 40.0;
//俯视角
// auto lookfrom = point3(270, 554, 270);
// auto lookat = point3(270, 0, 270);
// vec3 vup(1, 0, 0);
// auto vfov = 90.0;

auto dist_to_focus = 10.0;
auto aperture = 0.0;
int image_width = static_cast<int>(image_height * aspect_ratio);
camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);

// shared_ptr<hittable> light = make_shared<xz_rect>(213, 343, 227, 332, 554, 
//     shared_ptr<material>());
//shared_ptr<hittable> light = 
        //make_shared<sphere>(point3(190, 90, 190), 90, shared_ptr<material>());
// hittable_list world = final();
// auto lookfrom = point3(478, 278, -600);
// auto lookat = point3(278, 278, 0);
/*
hittable_list random_scene() {
    hittable_list world;
    auto checker = make_shared<lambertian>(
        make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9)));
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

    auto Diffuse = make_shared<diffuse>(color(1, 1, 1));
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, Diffuse));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, Diffuse));

    auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

    return objects;
}
*/
hittable_list cornell_box() {
    hittable_list objects;

    
    auto red = make_shared<lambertian>(color(244,164,96)/256);
    auto purple = make_shared<lambertian>(color(114,70,129)/255.0);
    auto blue = make_shared<lambertian>(color(135,206,250)/255.0);
    auto kaqi = make_shared<lambertian>(color(89,183,107)/255.0);
    auto toma = make_shared<lambertian>(color(253,245,230)/255.0);
    auto orange = make_shared<lambertian>(color(255,165,0)/255.0);
    auto micro = make_shared<Mro_lambertian>(color(1.0f, 1.0f, 1.0f));
    micro->Kd = color(1,1,1);
    auto white = make_shared<lambertian>(color(1.0f, 1.0f, 1.0f));
    auto green = make_shared<lambertian>(color(0.14f, 0.45f, 0.091f));
    auto light = make_shared<diffuse_light>(color(8.0f * color(0.747f+0.058f, 0.747f+0.258f, 0.747f) + 15.6f * color(0.740f+0.287f,0.740f+0.160f,0.740f) + 18.4f *color(0.737f+0.642f,0.737f+0.159f,0.737f)));
    
    // for (int j = 0; j < 1000; j++) {
    //     auto albedo = color::random() * color::random();
    //     objects.add(make_shared<sphere>(vec3::random(0,150)+vec3(390,330,200), 10 , make_shared<lambertian>(albedo)));
    // }

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, purple));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 550, 0, 550, 0, blue));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, green));

    // shared_ptr<hittable> box0 =
    //     make_shared<box>(point3(0, 0, 0), point3(150, 150, 150), orange);
    // box0 = make_shared<translate>(box0, vec3(390,330,200));
    // objects.add(box0);
    
    shared_ptr<hittable> box1 =
        make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), orange);
    box1 = make_shared<rotate_y>(box1, 20);
    box1 = make_shared<translate>(box1, vec3(295, 0, 295));
    objects.add(box1);
    shared_ptr<hittable> box2 =//tall
        make_shared<box>(point3(0, 0, 0), point3(145, 330, 145), micro);
    box2 = make_shared<rotate_y>(box2, -50);
    box2 = make_shared<translate>(box2, vec3(150, 0, 140));
    objects.add(box2);  

    auto Metal = make_shared<metal>(color(0.8, 0.85, 0.88), 0);
    auto glass = make_shared<dielectric>(1.5);

    objects.add(make_shared<sphere>(point3(82,165,82)+vec3(310, 60, 260), 60 , Metal));
    objects.add(make_shared<sphere>(point3(305, 90, 140), 90 , glass));

    return objects;
}
/*
hittable_list final() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1,101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    objects.add(make_shared<sphere>(point3(260, 150, 45), 50,
make_shared<dielectric>(1.5))); objects.add(make_shared<sphere>( point3(0, 150, 145), 50,
make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto emat = make_shared<diffuse>(color(color(1.00, 0.71, 0.29)));
    objects.add(make_shared<sphere>(point3(400,200,400), 100, emat));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0,165), 10, white));
    }

    objects.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2), 15),
            vec3(-100,270,395)
        )
    );

    return objects;
}
*/

color ray_color(const ray &r, const color &background, const hittable &world,
                hittable_list lights, int depth) {
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

    color emit = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
    scatter_record srec;

    if (!rec.mat_ptr->scatter(r, rec, srec)) {  // hit the light
        return emit;
    }

    if(srec.is_specular){
        return  srec.attenuation * ray_color(srec.specular_ray, background, world, lights, depth - 1)  / RR;
    }
    //scattered一开始表示随机采样或者镜面反射 后来被我们替换成mix_pdf采样
    vec3 direction;
    double pdf_val;
    if(!lights.objects.size()){
        shared_ptr<pdf> pdf = srec.pdf_ptr;
        direction = pdf->generate();
        pdf_val = pdf->value(direction);
        srec.specular_ray = ray(rec.p, direction);
    } else {
        mix_pdf mix(srec.pdf_ptr, make_shared<hittablelist_pdf>(lights, rec.p));
        direction = mix.generate();
        pdf_val = mix.value(direction);
        srec.specular_ray = ray(rec.p, direction);

    }
    if(rec.mat_ptr->id == 9){
        //cerr<<"Ok\n";
        srec.attenuation = //vec3(1,1,1);
            rec.mat_ptr->micro_attention(unit_vector(r.direction()),direction,rec.normal);
    }
    return emit + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, srec.specular_ray) *
                      ray_color(srec.specular_ray, background, world, lights, depth - 1) /
                      pdf_val / RR;
}

int main() {
    //要采样的面光源
    hittable_list lights;
    lights.add(make_shared<xz_rect>(213, 343, 227, 332, 554.0, nullptr));
    lights.add(make_shared<sphere>(point3(305, 90, 140), 90, nullptr));
    lights.add(make_shared<sphere>(point3(82,165,82)+vec3(310, 60, 260), 60,nullptr));
    cerr<<"\nnumber of sampling area:\t"<<lights.objects.size();


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
                    pixel_color += ray_color(r, background, world, lights, max_depth) /
                                   samples_per_pixel;
                }
                framebuffer[j * image_width + image_width - 1 - i] = pixel_color;
            }
            mtx.lock();
            progress++;
            diff = end - begin;
            cerr << "\rcurrent: " << fixed << setprecision(2)
                 << 100.0 * (progress) / (image_height) << "%   "
                 << "Time past: " << fixed << setprecision(3) << diff.count() << " s"
                 << flush;
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