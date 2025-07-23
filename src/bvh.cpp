#include "bvh.h"

#include <algorithm>
#include <iterator>
#include <vector>

#include "bounds3.h"
#include "intersection.h"
#include "primitive.h"
#include "vec3.h"

BVH::BVH(std::vector<Primitive *> &primitives)
    // BVH::BVH(std::vector<Primitive *>::iterator primitives_start,
    //          std::vector<Primitive *>::iterator primitives_end)
    : Primitive(nullptr) {
  // auto d = std::distance(primitives_start, primitives_end);
  // if (d < 4) {
  if (primitives.size() < 4) {
    // for (auto r = primitives_start; r < primitives_end; ++r) {
    for (auto p : primitives) {
      _primitives.push_back(p);
    }
  } else {

    // Determine the axis with the largest range of centroid
    Bounds3 centroid_bounds;
    // for (auto r = primitives_start; r != primitives_end; ++r) {
    for (auto r : primitives) {
      // centroid_bounds.expand((*r)->bounds().centroid());
      auto r_bounds = r->bounds();
      centroid_bounds.expand(r_bounds.centroid());
      _bounds = _bounds.u(r_bounds);
    }
    int axis = centroid_bounds.maximum_extent();
    Vec3 centroid = centroid_bounds.centroid();

    auto split_point = std::partition(
        // primitives_start, primitives_end, [axis, centroid](auto r) {
        primitives.begin(), primitives.end(), [axis, centroid](auto p) {
          float a = p->bounds().centroid().axis[axis];
          float b = centroid.axis[axis];
          return a < b;
        });
    // std::vector<Primitive *> primitives_a;
    // std::vector<Primitive *> primitives_b;
    // for (auto r : primitives) {
    //   if (r->bounds().centroid().axis[axis] < centroid.axis[axis]) {
    //     primitives_a.push_back(r);
    //   } else {
    //     primitives_b.push_back(r);
    //   }
    // }

    std::vector<Primitive *> primitives_a = {primitives.begin(), split_point};
    std::vector<Primitive *> primitives_b = {split_point, primitives.end()};
    _child_a = new BVH(primitives_a);
    _child_b = new BVH(primitives_b);
    // _child_a = new BVH(primitives_start, split_point);
    // _child_b = new BVH(split_point, primitives_end);
  }
}

BVH::~BVH() {
  if (_child_a)
    delete _child_a;

  if (_child_b)
    delete _child_b;
}

std::optional<Intersection> BVH::intersect(Ray &ray) const {
  std::vector<Intersection> inters;

  if (!_primitives.empty()) {

    for (auto p : _primitives) {
      if (p->bounds().intersect_b(ray)) {
        auto inter = p->intersect(ray);
        if (inter.has_value()) {
          inters.push_back(inter.value());
        }
      }
    }

  } else {
    if (_child_a && _child_a->_bounds.intersect_b(ray)) {
      auto inter_a = _child_a->intersect(ray);
      if (inter_a.has_value())
        inters.push_back(inter_a.value());
    }
    if (_child_b && _child_b->_bounds.intersect_b(ray)) {
      auto inter_b = _child_b->intersect(ray);
      if (inter_b.has_value())
        inters.push_back(inter_b.value());
    }
  }
  // return std::nullopt;
  // std::vector<Intersection> inters;

  // if (_child_a->bounds().intersect_b(ray)) {

  // }

  if (inters.empty()) {
    return std::nullopt;
  } else {
    return std::optional<Intersection>(
        *std::min_element(inters.begin(), inters.end()));
  }
}

float BVH::intersect_t(const Ray &ray) const { return -1.f; }

Bounds3 BVH::bounds() const { return _bounds; }