#ifndef RAY_H
#define RAY_H

#include <limits>

#include "vec3.h"

class Ray {
public:
    float t_min = 0.0001;
    float t_max = std::numeric_limits<float>::max();
    Vec3 origin, direction;

    Ray(Vec3 origin, Vec3 direction);
};

#endif // RAY_H
