#ifndef BVH_H
#define BVH_H

#include <vector>

#include "bounds3.h"
#include "primitive.h"
#include "vec3.h"

// class BVHNode {
// public:
//   BVHNode *child_a = nullptr;
//   BVHNode *child_b = nullptr;
//   Bounds3 _bounds;
//   std::vector<Primitive> _primitives;
// };

class BVH : public Primitive {
public:
  BVH(std::vector<Primitive *> &primitives);
  // BVH(std::vector<Primitive *>::iterator primitives_start,
  //     std::vector<Primitive *>::iterator primitives_end);
  ~BVH();
  std::optional<Intersection> intersect(Ray &ray) const override;
  float intersect_t(const Ray &ray) const override;
  // void point_on_surface(Vec3* point, Vec3* normal) const override;
  Bounds3 bounds() const override;

private:
  Bounds3 _bounds;
  std::vector<Primitive *> _primitives;
  BVH *_child_a = nullptr;
  BVH *_child_b = nullptr;
};

#endif // BVH_H