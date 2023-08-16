#ifndef PLANE_H
#define PLANE_H

#include "intersection.h"
#include "ray.h"
#include "primitive.h"
#include "vec3.h"

class Plane : public Renderable {
public:
    Vec3 p0;
    Vec3 normal;
    float d;

    Plane(Vec3 p0, Vec3 normal, Material const* material);
    std::optional<Intersection> intersect(const Ray& ray) const;
};

#endif // PLANE_H