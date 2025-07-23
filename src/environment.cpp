#include "environment.h"

#include <limits>

#include "vec3.h"

Environment::Environment(Material const *material) : Primitive(material) {}

std::optional<Intersection> Environment::intersect(Ray &ray) const {
  float t = intersect_t(ray);
  if (!t || t < ray.t_min || t > ray.t_max) {
    return std::nullopt;
  }

  Vec3 inter_point = ray.direction() * t;
  Vec3 inter_normal = -ray.direction();

  return std::optional<Intersection>{
      Intersection(t, inter_point, inter_normal, this->material)};
}

float Environment::intersect_t(const Ray &ray) const {
  return std::numeric_limits<float>::max();
}

// TODO: precalc in constructor
Bounds3 Environment::bounds() const {
  Bounds3 b;
  b.expand(Vec3(std::numeric_limits<float>::max()));
  b.expand(Vec3(std::numeric_limits<float>::min()));

  return b;
}