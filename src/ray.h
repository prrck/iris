#ifndef RAY_H
#define RAY_H

#include <limits>

#include "vec3.h"

class Ray {
public:
  float t_min = 0.0001;
  float t_max = std::numeric_limits<float>::max();

  Ray(Vec3 origin, Vec3 direction);
  const Vec3 &origin() const { return _origin; }
  const Vec3 &direction() const { return _direction; }
  const Vec3 &inv_dir() const { return _inv_dir; }

private:
  Vec3 _origin;
  Vec3 _direction;
  Vec3 _inv_dir;
};

#endif // RAY_H
