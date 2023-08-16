#ifndef BVH_H
#define BVH_H

#include <vector>

#include "bounds3.h"
#include "primitive.h"
#include "vec3.h"

// Heavily inspired by
// https://www.pbr-book.org/3ed-2018/Primitives_and_Intersection_Acceleration/Bounding_Volume_Hierarchies.

class BVHPrimitiveInfo {
    BVHPrimitiveInfo(size_t prim_nb, const Bounds3& bounds)
        : prim_nb(prim_nb)
        , bounds(bounds)
        , centroid(bounds.centroid())
    {
    }

    size_t prim_nb;
    Bounds3 bounds;
    Vec3 centroid;
};

class BVHAccel {
    BVHAccel(const std::vector<Renderable*>& prims, int max_prims_by_node);

private:
    const int _max_prims_by_node;
    std::vector<Renderable*> _primitives;
};

#endif // BVH_H