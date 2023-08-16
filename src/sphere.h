#ifndef SPHERE_H
#define SPHERE_H

#include "intersection.h"
#include "primitive.h"
#include "ray.h"
#include "vec3.h"

class Sphere : public Renderable {
public:
    Vec3 origin;
    float radius;

    Sphere(Vec3 origin, float radius, Material const* material);
    std::optional<Intersection> intersect(const Ray& ray) const override;
    float intersect_t(const Ray& ray) const override;
    void point_on_surface(Vec3* point, Vec3* normal) const override;
    Bounds3 bounds() const override;
};

#endif // SPHERE_H
