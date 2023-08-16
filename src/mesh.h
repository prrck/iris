#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <assimp/mesh.h>

#include "bounds3.h"
#include "material.h"
#include "primitive.h"
#include "vec3.h"

struct IndexedTriangle {
    union {
        struct
        {
            float v0, v1, v2;
        };
        float v[3];
    };

    union {
        struct
        {
            float n0, n1, n2;
        };
        float n[3];
    };
};

class Mesh : public Renderable {
public:
    Mesh(const aiMesh& ai_mesh, Material const* material);
    Mesh(Material const* material);
    std::optional<Intersection> intersect(const Ray& ray) const override;
    float intersect_t(const Ray& ray) const override;
    void point_on_surface(Vec3* point, Vec3* normal) const override;
    Bounds3 bounds() const override;

private:
    Bounds3 _bounds;
    std::vector<IndexedTriangle> _triangles;
    std::vector<Vec3> _vertices;
    std::vector<Vec3> _normals;

    float triangle_intersect(
        const Ray& ray,
        const IndexedTriangle& tri,
        Vec3& inter_point,
        Vec3& normal) const;
    float triangle_intersect_t(const Ray& ray, const IndexedTriangle& tri) const;
};

#endif // MESH_H