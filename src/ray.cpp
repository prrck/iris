#include "ray.h"
#include "vec3.h"

Ray::Ray(Vec3 origin, Vec3 direction) : _origin(origin), _direction(direction) {
  _inv_dir = Vec3(1) / direction;
}
