#include "sphere.h"

#include <cmath>

#include "random.h"

Sphere::Sphere(Vec3 origin, float radius, Material const* material)
    : Renderable(material)
    , origin(origin)
    , radius(radius)
{
}

std::optional<Intersection> Sphere::intersect(const Ray& ray) const
{
    // Vec3 oo = ray.origin - this->origin;

    // float b = 2.0 * ray.direction.dot(oo);
    // float oo_length = oo.length();
    // float c = oo_length * oo_length - this->radius * this->radius;

    // float delta = b * b - 4.0 * c;

    // float t = -1.0;
    // if (delta > 0) {
    //     float delta_root = sqrt(delta);
    //     float t0 = (-b - delta_root) / 2.0;
    //     float t1 = (-b + delta_root) / 2.0;

    //     t = std::min(t0, t1);
    // } else if (delta == 0.0) {
    //     t = -b / 2.0;
    // } else {
    //     return std::nullopt;
    // }

    // if (t < ray.t_min || t > ray.t_max) {
    //     return std::nullopt;

    float t = intersect_t(ray);
    if (!t) {
        return std::nullopt;
    }

    Vec3 inter_point = ray.origin + ray.direction * t;
    Vec3 inter_normal = (inter_point - this->origin) / this->radius;

    return std::optional<Intersection> {
        Intersection(t, inter_point, inter_normal, this->material)
    };
}

float Sphere::intersect_t(const Ray& ray) const
{
    Vec3 oo = ray.origin - this->origin;

    float b = 2.0 * ray.direction.dot(oo);
    float oo_length = oo.length();
    float c = oo_length * oo_length - this->radius * this->radius;

    float delta = b * b - 4.0 * c;

    float t = -1.0;
    if (delta > 0) {
        float delta_root = sqrt(delta);
        float t0 = (-b - delta_root) / 2.0;
        float t1 = (-b + delta_root) / 2.0;

        t = std::min(t0, t1);
    } else if (delta == 0.0) {
        t = -b / 2.0;
    } else {
        return false;
    }

    if (t < ray.t_min || t > ray.t_max) {
        return false;
    }

    return t;
}

void Sphere::point_on_surface(Vec3* point, Vec3* normal) const
{
    *normal = sample_sphere();
    *point = origin + *normal * radius;
}

Bounds3 Sphere::bounds() const
{
    Bounds3 b;
    b.expand(origin);
    b.add_margin(radius);

    return b;
}