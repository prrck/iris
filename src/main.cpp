#include <ctime>

#include <iostream>
#include <numbers>
#include <optional>
#include <sstream>
#include <string>

#include <omp.h>
#include <spdlog/spdlog.h>

#include "commandLine.h"

#include "camera.h"
#include "environment.h"
#include "image.h"
#include "intersection.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "scene.h"
#include "sphere.h"
#include "vec3.h"

Ray generate_secondary_ray(const Intersection &inter) {
  Vec3 direction = sample_cosine_hemisphere(inter.normal);
  // Vec3 direction = sample_hemisphere(inter.normal);
  return Ray(inter.point, direction);
}

Vec3 trace_path(Ray &ray, const Scene &scene, int depth) {
  if (depth <= 0) {
    return Vec3(0.0);
  }

  // find intersections
  std::optional<Intersection> opt_inter = scene.find_closest_inter(ray);
  if (!opt_inter) {
    return Vec3(0.0);
  }
  Intersection &inter = opt_inter.value();

  Ray new_ray = generate_secondary_ray(inter);

  constexpr float prob = 1.0f / (2.0f * float(std::numbers::pi));

  // Compute the BRDF for this ray (assuming Lambertian reflection)
  // float cos_theta = std::max(new_ray.direction().dot(inter.normal), 0.0);
  // Vec3 BRDF = inter.material->color / float(M_PI);
  Vec3 BRDF = inter.material->color / (2.0f * float(std::numbers::pi));

  // Recursively trace reflected light sources.
  Vec3 incoming = trace_path(new_ray, scene, depth - 1);

  // Apply the Rendering Equation here.
  // return BRDF * incoming * cos_theta / prob;
  return inter.material->emittance +
         (BRDF * incoming / prob); // skip cos becaus cosine sampling
}

// Only return the color of the closest object.
Vec3 trace_path_color(Ray &ray, const Scene &scene) {
  // find intersections
  std::optional<Intersection> opt_inter = scene.find_closest_inter(ray);
  if (opt_inter) {
    Intersection &inter = opt_inter.value();
    return Vec3(inter.material->color);
  } else {
    return Vec3(0.0);
  }
}

// Only return the normal of the closest object.
Vec3 trace_path_normal(Ray &ray, const Scene &scene) {
  // find intersections
  std::optional<Intersection> opt_inter = scene.find_closest_inter(ray);
  if (opt_inter) {
    Intersection &inter = opt_inter.value();
    return inter.normal;
  } else {
    return Vec3(0.0);
  }
}

// Simpe lambert shading for geometry preview.
Vec3 trace_path_preview(Ray &ray, const Scene &scene) {
  // find intersections
  std::optional<Intersection> opt_inter = scene.find_closest_inter(ray);
  if (opt_inter) {
    Intersection &inter = opt_inter.value();
    return Vec3(inter.normal.dot(Vec3(.2f, .7f, .1f)));
  } else {
    return Vec3(0.0);
  }
}

int main(int argc, char *argv[]) {
  std::string filepath("test.exr");
  int width = 1024;
  int height = 512;
  float fov = 1.0;
  int max_samples = 256;
  int max_recursions = 10;
  bool print_help = false;

  // Command line parsing
  CommandLine args("iris renderer.");
  args.addArgument({"--output", "-o"}, &filepath,
                   "File path of the rendered image");
  args.addArgument({"--width", "-w"}, &width, "Width of the rendered image");
  args.addArgument({"--height", "-h"}, &height, "Height of the rendered image");
  args.addArgument({"--fov", "-f"}, &fov, "Field of view");
  args.addArgument({"--samples", "-s"}, &max_samples, "Samples per pixel");
  args.addArgument({"--recursions", "-r"}, &max_recursions,
                   "Recursions per pixel");
  args.addArgument({"--help"}, &print_help, "Print help message");

  try {
    args.parse(argc, argv);
  } catch (std::runtime_error const &e) {
    std::cerr << e.what() << std::endl;
    args.printHelp();
    return -1;
  }
  std::cout << "print_help: " << print_help << std::endl;
  if (print_help) {
    args.printHelp();
    return 0;
  }

  const std::time_t now = std::time(nullptr);
  const std::tm now_local = *std::localtime(&now);
  const int NB_PIXELS = width * height;
  const int NB_PIXELS_ONE_PERCENT = NB_PIXELS / 100;

  Image image(width, height);
  // Camera camera(Vec3(0.0, 0.0, 3.0), Vec3(0.0, 0.0, -1.0), width, height,
  // fov);
  Camera camera(Vec3(5.0, 3.0, 8.0), Vec3(-1.0, 0.7, 0.0), width, height, fov);
  // Camera camera(Vec3(0.531713, -7.81804, 1.53039),
  //               Vec3(2.45148, 3.43654, 0.963126), width, height, fov);

  Material emissive(Vec3(0.0), Vec3(5.0));
  Material emissive_env(Vec3(0.0), Vec3(1.0));
  Material emissive_red(Vec3(0.0), Vec3(2, 1, 1));
  Material emissive_blue(Vec3(0.0), Vec3(1, 1, 2));

  // Sphere sphere_emissive(Vec3(2.45148, 3.43654, 0.963126), 0.5, &emissive);
  Sphere sphere_emissive(Vec3(1, 5, 0), 2, &emissive);
  Sphere sphere_emissive_red(Vec3(-2, 5, 0), 2, &emissive_red);
  Sphere sphere_emissive_blue(Vec3(1, 7, 0), 2, &emissive_blue);

  Environment env(&emissive_env);

  // Scene scene(
  // "/home/pierrick/Projets/iris/meshes/aligned-cubes/aligned-cubes.obj");
  Scene scene("/home/pierrick/Projets/iris/meshes/porsche.obj");
  // Scene scene("/home/pierrick/Projets/iris/meshes/pony_cartoon.obj");
  // Scene scene("/home/pierrick/Projets/iris/meshes/room/room.blend");
  // scene.add(&sphere_emissive);
  // scene.add(&sphere_emissive_red);
  // scene.add(&sphere_emissive_blue);
  scene.add(&env);
  scene.build_bvh();

  // int progress = 0;
  int nb_pixels_done = 0;

  spdlog::info("Start rendering...");

#pragma omp parallel for schedule(dynamic, 4)
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float x_norm = x / (float)width;
      float y_norm = y / (float)height;
      Vec3 color(0.0);

      // Special case if max_samples is set to 0:
      // only get color without shading.
      if (max_samples == 0) {
        Ray ray = camera.generate_primary_ray(x_norm, y_norm, false);
        // color = trace_path_normal(ray, scene);
        color = trace_path_preview(ray, scene);
      } else {
        for (int s = 0; s < max_samples; ++s) {
          Ray ray = camera.generate_primary_ray(x_norm, y_norm);
          color += trace_path(ray, scene, max_recursions);
          // color += trace_ray(ray, scene);
        }

        color /= float(max_samples);
      }

      // Don't apply gamma correction if max_samples == 0 (rendering of
      // normals).
      image.put_pixel(x, height - y - 1, color);

      nb_pixels_done += 1;
      if (nb_pixels_done % NB_PIXELS_ONE_PERCENT == 0) {
        // std::cout << 100 * nb_pixels_done / NB_PIXELS << "%" << std::endl;
        int percent = 100 * nb_pixels_done / NB_PIXELS;
        spdlog::info("  Rendered {}%", percent);
      }
    }
  }

  spdlog::info("Rendering Done.");

  // Write image. add date and time to file name.
  char now_local_cstr[50];
  std::strftime(now_local_cstr, 50, "%F_%T", &now_local);
  std::stringstream ss;

  const auto pos = filepath.find_last_of(".");
  if (pos == std::string::npos) {
    ss << filepath << "_" << now_local_cstr << ".exr";
  } else {
    const std::string filename = filepath.substr(0, pos);
    const std::string extension = filepath.substr(pos, std::string::npos);

    ss << filename << "_" << now_local_cstr << extension;
  }
  const std::string dated_filepath = ss.str();

  image.write(dated_filepath);

  spdlog::info("Done.");
  return 0;
}
