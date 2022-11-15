 objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    // shared_ptr<hittable> box1 =
    //     make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    // box1 = make_shared<rotate_y>(box1, 15);
    // box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    // objects.add(box1);
    // shared_ptr<hittable> box2 =
    //     make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    // box2 = make_shared<rotate_y>(box2, -18);
    // box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    // objects.add(box2);  

    auto Metal = make_shared<metal>(color(0.8, 0.85, 0.88), 0);
    auto glass = make_shared<dielectric>(1.5);
    objects.add(make_shared<sphere>(point3(395,100,300), 50 , Micro));
    objects.add(make_shared<sphere>(point3(276,100,300), 50 , Micro1));
    objects.add(make_shared<sphere>(point3(157,100,300), 50 , Micro2));
    
    // auto Micro = make_shared<Mro_lambertian>(color(0.725f, 0.71f, 0.68f));
    // Micro->rough = 0.0;//Micro->Kd = vec3(0.725f, 0.71f, 0.68f);

    // auto Micro1 = make_shared<Mro_lambertian>(color(0.725f, 0.71f, 0.68f));
    // Micro1->rough = 0.4;//Micro1->Kd = vec3(0.725f, 0.71f, 0.68f);

    // auto Micro2 = make_shared<Mro_lambertian>(color(0.725f, 0.71f, 0.68f));
    // Micro2->rough = 0.8;Micro2->Kd = vec3(0.725f, 0.71f, 0.68f);
    //objects.add(make_shared<sphere>(point3(157,100,300), 100 , Micro1));