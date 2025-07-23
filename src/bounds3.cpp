#include "bounds3.h"
#include "vec3.h"

#include <algorithm>
#include <limits>

Bounds3::Bounds3() {
  // Create degenerate bounds.
  min = Vec3(std::numeric_limits<float>::max());
  max = Vec3(std::numeric_limits<float>::lowest());
}

Bounds3::Bounds3(const Vec3 &min, const Vec3 &max) : min(min), max(max) {}

void Bounds3::expand(const Vec3 &p) {
  min.x = std::min(min.x, p.x);
  min.y = std::min(min.y, p.y);
  min.z = std::min(min.z, p.z);

  max.x = std::max(max.x, p.x);
  max.y = std::max(max.y, p.y);
  max.z = std::max(max.z, p.z);
}

Bounds3 Bounds3::add_margin(float amount) const {
  Bounds3 new_bounds = Bounds3(*this);

  new_bounds.min.x -= amount;
  new_bounds.min.y -= amount;
  new_bounds.min.z -= amount;
  new_bounds.max.x += amount;
  new_bounds.max.y += amount;
  new_bounds.max.z += amount;

  return new_bounds;
}

Bounds3 Bounds3::u(const Bounds3 &o) const {
  Vec3 union_min(std::min(this->min.x, o.min.x), std::min(this->min.y, o.min.y),
                 std::min(this->min.z, o.min.z));

  Vec3 union_max(std::max(this->max.x, o.max.x), std::max(this->max.y, o.max.y),
                 std::max(this->max.z, o.max.z));

  return Bounds3(union_min, union_max);
}

Bounds3 Bounds3::i(const Bounds3 &o) const {
  Vec3 inter_min(std::max(this->min.x, o.min.x), std::max(this->min.y, o.min.y),
                 std::max(this->min.z, o.min.z));

  Vec3 inter_max(std::min(this->max.x, o.max.x), std::min(this->max.y, o.max.y),
                 std::min(this->max.z, o.max.z));

  return Bounds3(inter_min, inter_max);
}

Vec3 Bounds3::diagonal() const { return max - min; }

Vec3 Bounds3::centroid() const { return (min + max) * 0.5f; }

float Bounds3::surface_area() const {
  Vec3 diag = diagonal();
  return 2 * (diag.x * diag.y + diag.y * diag.z + diag.z * diag.x);
}

int Bounds3::maximum_extent() const {
  float max_extent = 0.f;
  int max_axis = 0;

  for (int i = 0; i < 3; ++i) {
    float extent = this->max.axis[i] - this->min.axis[i];
    if (extent > max_extent) {
      max_extent = extent;
      max_axis = i;
    }
  }

  return max_axis;
}

// bool Bounds3::intersect_b(const Ray &ray, float *t0, float *t1) const {
bool Bounds3::intersect_b(const Ray &ray) const {
  float temp_t0 = ray.t_min;
  float temp_t1 = ray.t_max;

  // For every axis, get intersections with two planes.
  const Vec3 ray_inv_dir = ray.inv_dir();
  for (int axis = 0; axis < 3; ++axis) {
    float t_near = (min[axis] - ray.origin()[axis]) * ray_inv_dir[axis];
    float t_far = (max[axis] - ray.origin()[axis]) * ray_inv_dir[axis];

    if (t_near > t_far)
      std::swap(t_near, t_far);

    temp_t0 = std::max(t_near, temp_t0);
    temp_t1 = std::min(t_far, temp_t1);
    if (temp_t0 > temp_t1)
      return false;
  }

  // if (t0)
  //   *t0 = temp_t0;
  // if (t1)
  //   *t1 = temp_t1;

  return true;
}