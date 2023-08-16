#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

class Material {
public:
    Vec3 color;
    Vec3 emittance;

    Material(Vec3 color, Vec3 emittance);
};

#endif // MATERIAL_H
