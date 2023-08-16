#include "mesh.h"

#include <algorithm>
#include <iostream>

#include <glog/logging.h>

#include "random.h"

Mesh::Mesh(const aiMesh& ai_mesh, const Material* material)
    : Renderable(material)
{
    LOG(INFO) << "Loading mesh \"" << ai_mesh.mName.C_Str() << "\"..." << std::endl;

    _triangles.resize(ai_mesh.mNumFaces);
    _vertices.resize(ai_mesh.mNumVertices);
    _normals.resize(ai_mesh.mNumVertices);

    int currentTriangle = 0;
    int currentVertex = 0;
    int currentNormal = 0;

    for (int f = 0; f < ai_mesh.mNumFaces; ++f, ++currentTriangle) {
        const aiFace face = ai_mesh.mFaces[f];

        IndexedTriangle& tri = _triangles[currentTriangle];

        for (int v = 0; v < 3; ++v) {
            const int idV = face.mIndices[v];
            tri.v[v] = idV;
            tri.n[v] = idV;
        }
    }

    for (int v = 0; v < ai_mesh.mNumVertices; ++v, ++currentVertex) {
        Vec3& vertex = _vertices[currentVertex];
        vertex.x = ai_mesh.mVertices[v].x;
        vertex.y = ai_mesh.mVertices[v].y;
        vertex.z = ai_mesh.mVertices[v].z;

        _bounds.expand(vertex);
    }

    for (int n = 0; n < ai_mesh.mNumVertices; ++n, ++currentNormal) {
        Vec3& normal = _normals[currentNormal];
        normal.x = ai_mesh.mNormals[n].x;
        normal.y = ai_mesh.mNormals[n].y;
        normal.z = ai_mesh.mNormals[n].z;

        // TODO: Is it correct that some normals are not normalized ???
        normal = normal.normalize();
    }
    // }
}

Mesh::Mesh(Material const* material)
    : Renderable(material)
{
    _vertices.resize(3);
    _normals.resize(1);
    _triangles.resize(1);

    _vertices[0] = Vec3(0, 0, 0);
    _vertices[1] = Vec3(0, 1, 0);
    _vertices[2] = Vec3(1, 0, 0);

    _normals[0] = Vec3(0, 0, -1);

    _triangles[0].n0 = 0;
    _triangles[0].n1 = 0;
    _triangles[0].n2 = 0;

    _triangles[0].v0 = 0;
    _triangles[0].v1 = 1;
    _triangles[0].v2 = 2;

    for (Vec3 v : _vertices) {
        _bounds.expand(v);
    }
}

float Mesh::triangle_intersect(
    const Ray& ray,
    const IndexedTriangle& tri,
    Vec3& inter_point,
    Vec3& normal) const
{
    // MOLLER_TRUMBORE
    const Vec3& v0 = _vertices[tri.v0];
    const Vec3& v1 = _vertices[tri.v1];
    const Vec3& v2 = _vertices[tri.v2];

    const Vec3& n0 = _normals[tri.n0];
    const Vec3& n1 = _normals[tri.n1];
    const Vec3& n2 = _normals[tri.n2];

    Vec3 e1 = v1 - v0;
    Vec3 e2 = v2 - v0;
    Vec3 pvec = ray.direction.cross(e2);
    float det = e1.dot(pvec);

    // TODO: problème de contour. Certaines faces sont transparantes.
    // determinant negative -> triangle backfacing
    // determinant close to zero -> triangle almost parallel to ray
    if (det == 0)
        return false;

    // std::cout << det << std::endl;

    float inv_det = 1 / det;

    Vec3 tvec = ray.origin - v0;
    float u = tvec.dot(pvec) * inv_det;
    if (u < 0 || u > 1)
        return false;

    Vec3 qvec = tvec.cross(e1);
    float v = ray.direction.dot(qvec) * inv_det;
    if (v < 0 || v + u > 1)
        return false;

    float t = e2.dot(qvec) * inv_det;
    if (t < ray.t_min || t > ray.t_max)
        return false;

    float w = 1.0 - u - v;

    normal = (n0 * w + n1 * u + n2 * v).normalize();
    // TODO: float SIDED should we keep it ?
    // flip normal if not facing the camera.
    if (normal.dot(ray.direction) > 0)
        normal = -normal;

    inter_point = ray.origin + ray.direction * t;
    return t;

    // GEOMETRIC
    // const Vec3& v0 = _vertices[tri.v0];
    // const Vec3& v1 = _vertices[tri.v1];
    // const Vec3& v2 = _vertices[tri.v2];

    // normal = _normals[tri.v0]; // TODO: only debug

    // // const Vec3& n0 = _normals[tri.n0];
    // // const Vec3& n1 = _normals[tri.n1];
    // // const Vec3& n2 = _normals[tri.n2];

    // const Vec3 v0v1 = v1 - v0;
    // const Vec3 v0v2 = v2 - v0;
    // // normal = v0v1.cross(v0v2).normalize();

    // float denom = normal.dot(ray.direction);

    // // Test if intersection with plane containing triangle.
    // if (denom != 0) {
    //     float d = -normal.dot(v0);
    //     float t = -(normal.dot(ray.origin) + d) / denom;

    //     // Test inter_point in front of ray.
    //     if (t > ray.T_MIN) {
    //         inter_point = ray.origin + ray.direction * t;
    //         // return t;

    //         Vec3 C;
    //         // Test if inter_point is inside triangle.
    //         // edge 0
    //         Vec3 edge0 = v1 - v0;
    //         Vec3 vp0 = inter_point - v0;
    //         C = edge0.cross(vp0);
    //         if (normal.dot(C) < 0)
    //             return false; // inter_point is on the right side

    //         // edge 1
    //         Vec3 edge1 = v2 - v1;
    //         Vec3 vp1 = inter_point - v1;
    //         C = edge1.cross(vp1);
    //         if (normal.dot(C) < 0)
    //             return false; // inter_point is on the right side

    //         // edge 2
    //         Vec3 edge2 = v0 - v2;
    //         Vec3 vp2 = inter_point - v2;
    //         C = edge2.cross(vp2);
    //         if (normal.dot(C) < 0)
    //             return false; // inter_point is on the right side;

    //         // TODO: float SIDED should we keep it ?
    //         // flip normal if not facing the camera.
    //         if (normal.dot(ray.direction) > 0)
    //             normal = -normal;

    //         return t;
    //     }
    // }

    // return false;
}

float Mesh::triangle_intersect_t(
    const Ray& ray,
    const IndexedTriangle& tri) const
{
    // MOLLER_TRUMBORE
    const Vec3& v0 = _vertices[tri.v0];
    const Vec3& v1 = _vertices[tri.v1];
    const Vec3& v2 = _vertices[tri.v2];

    const Vec3& n0 = _normals[tri.n0];
    const Vec3& n1 = _normals[tri.n1];
    const Vec3& n2 = _normals[tri.n2];

    Vec3 e1 = v1 - v0;
    Vec3 e2 = v2 - v0;
    Vec3 pvec = ray.direction.cross(e2);
    float det = e1.dot(pvec);

    // TODO: problème de contour. Certaines faces sont transparantes.
    // determinant negative -> triangle backfacing
    // determinant close to zero -> triangle almost parallel to ray
    if (det == 0)
        return false;

    // std::cout << det << std::endl;

    float inv_det = 1 / det;

    Vec3 tvec = ray.origin - v0;
    float u = tvec.dot(pvec) * inv_det;
    if (u < 0 || u > 1)
        return false;

    Vec3 qvec = tvec.cross(e1);
    float v = ray.direction.dot(qvec) * inv_det;
    if (v < 0 || v + u > 1)
        return false;

    return e2.dot(qvec) * inv_det;
}

std::optional<Intersection> Mesh::intersect(const Ray& ray) const
{
    std::vector<Intersection> inters;

    if (_bounds.intersect_b(ray, nullptr, nullptr)) {
        for (auto tri = _triangles.begin(); tri != _triangles.end(); ++tri) {
            Vec3 inter_point;
            Vec3 normal;

            const float t = triangle_intersect(ray, *tri, inter_point, normal);

            if (t) {
                inters.push_back(
                    Intersection(t, inter_point, normal, this->material));
            }
        }
    }

    if (inters.empty()) {
        return std::nullopt;
    } else {
        return std::optional<Intersection>(
            *std::min_element(inters.begin(), inters.end()));
    }
}

float Mesh::intersect_t(const Ray& ray) const
{
    float t_min = false;

    if (_bounds.intersect_b(ray, nullptr, nullptr)) {
        for (auto tri = _triangles.begin(); tri != _triangles.end(); ++tri) {
            const float t = triangle_intersect_t(ray, *tri);
            if (!t_min) {
                t_min = t;
            } else if (t_min && t < t_min) {
                t_min = t;
            }
        }
    }

    return t_min;
}

void Mesh::point_on_surface(Vec3* point, Vec3* normal) const
{
    // TODO: This is just a hack for now. All points are the center of bounding
    // box.

    *point = _bounds.centroid();
    *normal = sample_sphere();
}

Bounds3 Mesh::bounds() const
{
    return _bounds;
}