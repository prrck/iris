#ifndef PLANE_H
#define PLANE_H

#include "intersection.h"
#include "primitive.h"
#include "ray.h"
#include "vec3.h"

class Plane : public Primitive {
public:
  Vec3 p0;
  Vec3 normal;
  float d;

  Plane(Vec3 p0, Vec3 normal, Material const *material);
  std::optional<Intersection> intersect(Ray &ray) const;
};

#endif // PLANE_H