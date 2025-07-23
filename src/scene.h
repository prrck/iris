#ifndef SCENE_H
#define SCENE_H

#include <optional>
#include <vector>

#include "bvh.h"
#include "intersection.h"
#include "material.h"
#include "primitive.h"
#include "ray.h"

class Scene {
public:
  Scene(const std::string &filepath);
  ~Scene();

  void add(Primitive *Primitive);
  void build_bvh();
  const std::vector<Primitive *> &get_primitives() const { return _primitives; }
  const std::vector<Primitive *> &get_emissives() const { return _emissives; }
  std::optional<Intersection> find_closest_inter(Ray &ray) const;

private:
  Material _default_material;
  BVH *_bvh = nullptr;
  std::vector<Primitive *> _primitives;
  std::vector<Primitive *> _emissives;
};

#endif // SCENE_H
