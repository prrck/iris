#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <optional>

#include "bounds3.h"
#include "intersection.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

class Primitive {
public:
  // TODO:
  // material should not be set in main class.
  // We want to treat BVH as primitives but it does not have a material.
  // Material should probably be included in intersection object.
  Material const *material;

  Primitive(Material const *material);
  virtual ~Primitive() {};
  // TODO: should probably return vector or intersection instead.
  // would not a to treat the case of multiple intersections by object in
  // every intersect methods. See Mesh.intersect()
  virtual std::optional<Intersection> intersect(Ray &ray) const = 0;
  virtual float intersect_t(const Ray &ray) const = 0;
  // virtual void point_on_surface(Vec3 *point, Vec3 *normal) const = 0;
  virtual Bounds3 bounds() const = 0;
};
#endif // RENDERABLE_H
