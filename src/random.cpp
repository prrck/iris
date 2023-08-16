#include "random.h"

#include <random>

float rand_uniform_float()
{
    static std::default_random_engine re {};
    using Dist = std::uniform_real_distribution<float>;
    static Dist uid {};
    return uid(re, Dist::param_type { -1.0, 1.0 });
}

float rand_gaussian_float()
{
    static std::default_random_engine re {};
    using Dist = std::normal_distribution<float>;
    static Dist uid {};
    return uid(re, Dist::param_type { 0.0, 1.0 });
}

Vec3 sample_sphere()
{
    // TODO: Coordonnées polaires.
    Vec3 vec = Vec3(
        rand_gaussian_float(),
        rand_gaussian_float(),
        rand_gaussian_float());
    // rand_uniform_float(),
    // rand_uniform_float(),
    // rand_uniform_float());

    vec.normalize();

    return vec;
}

Vec3 sample_hemisphere(Vec3 normal)
{
    // Vec3 vec = Vec3(rand_gaussian_float(), rand_gaussian_float(), rand_gaussian_float());
    // vec.normalize();
    Vec3 vec = sample_sphere();

    if (normal.dot(vec) < 0) {
        vec.invert();
    }

    return vec;
}

// Vec3 sample_cosine_hemisphere(Vec3 normal) {
//     float x = 1.0;
//     float y = 1.0;
//     while (x * x + y * y > 1.0) {
//         x = rand_uniform_float() / 2.0;
//         y = rand_uniform_float() / 2.0;
//     }

//     // voir page 780 chapitre 13
//     float z = 1.0 - x * x - y * y;

//     Vec3 tan_x, tan_y;
//     normal.coordinate_system(&tan_x, &tan_y);

//     return tan_x * x + tan_y * y + normal * z;
// }

Vec3 sample_cosine_hemisphere(Vec3 normal)
{
    Vec3 vec = Vec3(normal + sample_sphere());
    vec.normalize();

    return vec;
}