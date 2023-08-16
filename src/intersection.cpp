#include "intersection.h"

#include <glog/logging.h>

Intersection::Intersection(float t, Vec3 point, Vec3 normal, Material const* material)
    : t(t)
    , point(point)
    , normal(normal)
    , material(material)
{
    if (1 - normal.length() > 1e-5)
        LOG(ERROR) << "Intersection normal is not normalized. " << normal;
}

bool Intersection::operator<(const Intersection& other) const
{
    return this->t < other.t;
}