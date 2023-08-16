#ifndef BOUNDS3_H
#define BOUNDS3_H

#include "ray.h"
#include "vec3.h"

class Bounds3 {
public:
    Bounds3();
    Bounds3(const Vec3& min, const Vec3& max);

    void expand(const Vec3& p);
    Bounds3 add_margin(float amount) const;

    // u is short for "union" which is a reserved key word in c++.
    Bounds3 u(const Bounds3& o) const;
    // i is short for "intersection".
    Bounds3 i(const Bounds3& o) const;

    Vec3 diagonal() const;
    Vec3 centroid() const;
    float surface_area() const;

    bool intersect_b(const Ray& ray, float* t0, float* t1) const;

private:
    Vec3 min;
    Vec3 max;
};

#endif // BOUNDS3_H
