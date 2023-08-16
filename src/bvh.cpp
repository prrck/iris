#include "bvh.h"

BVHAccel::BVHAccel(const std::vector<Renderable*>& prims, int max_prims_by_node)
    : _primitives(prims)
    , _max_prims_by_node(max_prims_by_node)
{
    if (_primitives.size() == 0)
        return;

    // Initialize primitiveInfo array.
    std::vector<BVHPrimitiveInfo> primitive_infos(_primitives.size());
    for (size_t i = 0; i < _primitives.size(); ++i) {
        primitive_infos[i] = BVHPrimitiveInfo(i, _primitives[i]->bounds());
    }
}