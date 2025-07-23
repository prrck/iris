#ifndef ENVIRONMENT_h
#define ENVIRONMENT_h

#include "intersection.h"
#include "primitive.h"
#include "ray.h"
#include "vec3.h"

class Environment : public Primitive {
public:
  Environment(Material const *material);
  std::optional<Intersection> intersect(Ray &ray) const override;
  float intersect_t(const Ray &ray) const override;
  // void point_on_surface(Vec3 *point, Vec3 *normal) const override;
  Bounds3 bounds() const override;
};

#endif // ENVIRONMENT_h
