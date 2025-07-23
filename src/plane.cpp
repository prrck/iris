#include "plane.h"

#include <cmath>

Plane::Plane(Vec3 p0, Vec3 normal, Material const *material)
    : Primitive(material), p0(p0), normal(normal) {
  this->d = -this->normal.dot(p0);
}

std::optional<Intersection> Plane::intersect(Ray &ray) const {
  float denom = this->normal.dot(ray.direction());

  if (denom != 0) {
    float t = -(this->normal.dot(ray.origin()) + d) / denom;

    if (t >= 0) {
      Vec3 inter_point = ray.origin() + ray.direction() * t;

      return std::optional<Intersection>{
          Intersection(t, inter_point, this->normal, this->material)};
    }
  }

  return std::nullopt;
}
