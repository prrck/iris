#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "material.h"
#include "vec3.h"

class Intersection {
public:
    float t;
    Vec3 point;
    Vec3 normal;
    Material const* material;

    // TODO: deduce intersection point from t and ray origin
    Intersection(float t, Vec3 point, Vec3 normal, Material const* material);
    bool operator<(const Intersection& other) const;
};

#endif // INTERSECTION_H
