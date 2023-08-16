#include <cmath>

#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <glog/logging.h>
#include <omp.h>

#include "commandLine.h"

#include "camera.h"
#include "image.h"
#include "intersection.h"
#include "material.h"
#include "mesh.h"
#include "plane.h"
#include "primitive.h"
#include "random.h"
#include "ray.h"
#include "scene.h"
#include "sphere.h"
#include "vec3.h"

Ray generate_secondary_ray(const Intersection& inter)
{
    Vec3 direction = sample_cosine_hemisphere(inter.normal);
    // Vec3 direction = sample_hemisphere(inter.normal);
    return Ray(inter.point, direction);
}

std::optional<Intersection> find_closest_inter(Ray& ray, const Scene& scene)
{
    std::optional<Intersection> closest_inter = std::nullopt;

    auto primitives = scene.get_primitives();
    for (auto primitive = primitives.begin(); primitive != primitives.end(); ++primitive) {
        std::optional<Intersection> inter = (*primitive)->intersect(ray);

        if (inter) {
            if (!closest_inter || inter.value().t < closest_inter.value().t) {
                closest_inter = inter;
                ray.t_max = inter.value().t;
            }
        }
    }

    return closest_inter;
}

Vec3 trace_path(Ray& ray, const Scene& scene, int depth)
{
    if (depth <= 0) {
        return Vec3(0.0);
    }

    // find intersections
    std::optional<Intersection> opt_inter = find_closest_inter(ray, scene);
    if (!opt_inter) {
        return Vec3(0.0);
    }
    Intersection& inter = opt_inter.value();

    Ray new_ray = generate_secondary_ray(inter);

    const float prob = 1.0f / (2.0f * float(M_PI));

    // Compute the BRDF for this ray (assuming Lambertian reflection)
    // float cos_theta = std::max(new_ray.direction.dot(inter.normal), 0.0);
    // Vec3 BRDF = inter.material->color / float(M_PI);
    Vec3 BRDF = inter.material->color / (2.0f * float(M_PI));

    // Recursively trace reflected light sources.
    Vec3 incoming = trace_path(new_ray, scene, depth - 1);

    // Apply the Rendering Equation here.
    // return BRDF * incoming * cos_theta / prob;
    return inter.material->emittance + (BRDF * incoming / prob); // skip cos becaus cosine sampling
}

// Vec3 trace_ray(Ray& ray, const Scene& scene)
// {
//     // find intersections
//     std::optional<Intersection> opt_inter = find_closest_inter(ray, scene);
//     if (!opt_inter) {
//         return Vec3(0.0);
//     }
//     Intersection& inter = opt_inter.value();

//     auto emissives = scene.get_emissives();
//     auto primitives = scene.get_primitives();

//     Vec3 radiance(0);
//     for (auto emissive = emissives.begin(); emissive < emissives.end(); ++emissive) {
//         Vec3 p_light, n_light;
//         (*emissive)->point_on_surface(&p_light, &n_light);
//         Vec3 to_light = (p_light - inter.point).normalize();
//         Ray shadow_ray(inter.point, to_light);

//         shadow_ray.t_max = (p_light - inter.point).length() - 0.001; // Avoid intersection with light and everything behind.

//         bool in_shadow = false;
//         // for (auto primitive = primitives.begin(); primitive < primitives.end(); ++primitive) {

//         //     // maybe check that t < t_max ?
//         //     float t = (*primitive)->intersect_t(shadow_ray);
//         //     if (t > 0.01 and t < shadow_ray.t_max) {
//         //         return Vec3(1, 1, 0);
//         //         in_shadow = true;
//         //         break;
//         //     }
//         // }

//         if (!in_shadow) {
//             radiance += inter.normal.dot(to_light) * (*emissive)->material->emittance;
//         }
//     }

//     // Div by 0 if no emissive in scene.
//     return inter.material->emittance + radiance / emissives.size();
// }

// Only return the color of the closest object.
Vec3 trace_path_color(Ray& ray, const Scene& scene)
{
    // find intersections
    std::optional<Intersection> opt_inter = find_closest_inter(ray, scene);
    if (opt_inter) {
        Intersection& inter = opt_inter.value();
        return Vec3(inter.material->color);
    } else {
        return Vec3(0.0);
    }
}

// Only return the normal of the closest object.
Vec3 trace_path_normal(Ray& ray, const Scene& scene)
{
    // find intersections
    std::optional<Intersection> opt_inter = find_closest_inter(ray, scene);
    if (opt_inter) {
        Intersection& inter = opt_inter.value();
        return inter.normal;
    } else {
        return Vec3(0.0);
    }
}

int main(int argc, char* argv[])
{
    std::string filepath("test.exr");
    int width = 1024;
    int height = 512;
    float fov = 1.0;
    int max_samples = 256;
    int max_recursions = 10;
    float print_help = false;

    // Command line parsing
    CommandLine args("iris renderer.");
    args.addArgument({ "-o", "--output" }, &filepath, "File path of the rendered image");
    args.addArgument({ "-w", "--width" }, &width, "Width of the rendered image");
    args.addArgument({ "-h", "--height" }, &height, "Height of the rendered image");
    args.addArgument({ "-f", "--fov" }, &fov, "Field of view");
    args.addArgument({ "-s", "--samples" }, &max_samples, "Samples per pixel");
    args.addArgument({ "-r", "--recursions" }, &max_recursions, "Recursions per pixel");
    args.addArgument({ "-h", "--help" }, &max_recursions, "Print help message");

    try {
        args.parse(argc, argv);
    } catch (std::runtime_error const& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    std::cout << "print_help: " << print_help << std::endl;
    // if (print_help) {
    args.printHelp();
    //     return 0;
    // }

    // google::InitGoogleLogging(argv[0]);

    const int NB_PIXELS = width * height;
    const int NB_PIXELS_ONE_PERCENT = NB_PIXELS / 100;

    Image image(width, height);
    // Camera camera(Vec3(0.0, 0.0, 3.0), Vec3(0.0, 0.0, -1.0), width, height, fov);
    Camera camera(Vec3(5.0, 3.0, 8.0), Vec3(-1.0, 0.7, 0.0), width, height, fov);

    Material emissive(Vec3(0.0), Vec3(5.0));
    Material emissive_red(Vec3(0.0), Vec3(2, 1, 1));
    Material emissive_blue(Vec3(0.0), Vec3(1, 1, 2));

    Sphere sphere_emissive(Vec3(1, 5, 0), 2, &emissive);
    Sphere sphere_emissive_red(Vec3(-2, 5, 0), 2, &emissive_red);
    Sphere sphere_emissive_blue(Vec3(1, 7, 0), 2, &emissive_blue);

    Scene scene("/home/pierrick/Projets/iris/meshes/porsche.obj");
    scene.add(&sphere_emissive);
    scene.add(&sphere_emissive_red);
    scene.add(&sphere_emissive_blue);

    // int progress = 0;
    int nb_pixels_done = 0;

// TODO: Get rid of openMP. Add support for selecting number of Threads.
#pragma omp parallel for schedule(dynamic, 1)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float x_norm = x / (float)width;
            float y_norm = y / (float)height;
            Vec3 color(0.0);

            // Special case if max_samples is set to 0:
            // only get color without shading.
            if (max_samples == 0) {
                Ray ray = camera.generate_primary_ray(x_norm, y_norm, false);
                color = trace_path_normal(ray, scene);
            } else {
                for (int s = 0; s < max_samples; ++s) {
                    Ray ray = camera.generate_primary_ray(x_norm, y_norm);
                    color += trace_path(ray, scene, max_recursions);
                    // color += trace_ray(ray, scene);
                }

                color /= float(max_samples);
            }

            // Don't apply gamma correction if max_samples == 0 (rendering of normals).
            image.put_pixel(x, height - y - 1, color);

            nb_pixels_done += 1;
            if (nb_pixels_done % NB_PIXELS_ONE_PERCENT == 0) {
                std::cout << 100 * nb_pixels_done / NB_PIXELS << "%" << std::endl;
            }
        }
    }

    image.write(filepath);
    std::cout << "Wrote image \"" << filepath << "\"" << std::endl;

    return 0;
}
