#include <iostream>

#include "model/concurrency/safe_stream.h"
#include "utility/defines.h"

#include "model/camera.h"
#include "model/scene.h"
#include "model/image.h"
#include "utility/stopwatch.h"
#include "model/materials/lambertian.h"
#include "model/materials/reflective.h"
#include "model/materials/grass.h"
#include "model/materials/dielectric.h"

void random_scene(std::vector<shared_ptr<Hittable>> &objects) {

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    objects.push_back(make_shared<Sphere>(Point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = Random::NextNumber();
            Point3 center(a + 0.9*Random::NextNumber(), 0.2, b + 0.9*Random::NextNumber());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> Sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::Random() * Color::Random();
                    Sphere_material = make_shared<Lambertian>(albedo);
                     objects.push_back(make_shared<Sphere>(center, 0.2, Sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::Random(0.5, 1);
                    auto fuzz = Random::NextNumber(0, 0.5);
                    Sphere_material = make_shared<Metal>(albedo, fuzz);
                    objects.push_back(make_shared<Sphere>(center, 0.2, Sphere_material));
                } else {
                    // glass
                    Sphere_material = make_shared<Dielectric>(1.5);
                    objects.push_back(make_shared<Sphere>(center, 0.2, Sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    objects.push_back(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    objects.push_back(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    objects.push_back(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));
}

int main() {
    Stopwatch t;
    t.reset();

    //image
    Image img;

    // World
    std::vector<shared_ptr<Hittable>> objects;
    random_scene(objects);
    Scene world(objects);

    //camera
    Point3 lookfrom(13,2,3);
    Point3 lookat(0,0,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, 20, img.aspect_ratio, aperture, dist_to_focus);

    cam.Draw(world, img);

    img.Save();

    std::cerr << "\nDone in " << t.diff() << " seconds"<<std::flush;

    return 0;
}
