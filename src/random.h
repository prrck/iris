#include "vec3.h"

float rand_uniform_float();
float rand_gaussian_float();

Vec3 sample_sphere();
Vec3 sample_hemisphere(Vec3 normal);
Vec3 sample_cosine_hemisphere(Vec3 normal);