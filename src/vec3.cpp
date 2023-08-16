#include "vec3.h"

#include <cassert>
#include <cmath>

Vec3::Vec3(float v)
    : x(v)
    , y(v)
    , z(v)
{
}

Vec3::Vec3(float x, float y, float z)
    : x(x)
    , y(y)
    , z(z)
{
}

Vec3::operator bool() const
{
    return x || y || z;
}

bool Vec3::operator==(const Vec3& other) const
{
    return (this->x == other.x) and (this->y == other.y) and (this->z == other.z);
}

Vec3 Vec3::operator+(const Vec3& other) const
{
    float x = this->x + other.x;
    float y = this->y + other.y;
    float z = this->z + other.z;

    return Vec3(x, y, z);
}

void Vec3::operator+=(const Vec3& other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
}

Vec3 Vec3::operator-(const Vec3& other) const
{
    float x = this->x - other.x;
    float y = this->y - other.y;
    float z = this->z - other.z;

    return Vec3(x, y, z);
}

Vec3 Vec3::operator-() const
{
    return Vec3(-x, -y, -z);
}

void Vec3::operator-=(const Vec3& other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
}

Vec3 Vec3::operator*(float scalar) const
{
    float x = this->x * scalar;
    float y = this->y * scalar;
    float z = this->z * scalar;

    return Vec3(x, y, z);
}

Vec3 Vec3::operator*(const Vec3& other) const
{
    float x = this->x * other.x;
    float y = this->y * other.y;
    float z = this->z * other.z;

    return Vec3(x, y, z);
}

Vec3 Vec3::operator/(float scalar) const
{
    float x = this->x / scalar;
    float y = this->y / scalar;
    float z = this->z / scalar;

    return Vec3(x, y, z);
}

Vec3 Vec3::operator/(const Vec3& other) const
{
    float x = this->x / other.x;
    float y = this->y / other.y;
    float z = this->z / other.z;

    return Vec3(x, y, z);
}

void Vec3::operator/=(float scalar)
{
    // One div instead of three.
    float inverse = 1 / scalar;

    this->x *= inverse;
    this->y *= inverse;
    this->z *= inverse;
}

float Vec3::operator[](int i) const
{
    assert(i >= 0 && i <= 2);
    if (i == 0)
        return x;
    if (i == 1)
        return y;
    return z;
}

Vec3 Vec3::cross(const Vec3& other) const
{
    return Vec3(
        (y * other.z) - (z * other.y),
        (z * other.x) - (x * other.z),
        (x * other.y) - (y * other.x));
}

float Vec3::dot(Vec3 other) const
{
    float x = this->x * other.x;
    float y = this->y * other.y;
    float z = this->z * other.z;

    return x + y + z;
}

float Vec3::length() const
{
    return sqrt(x * x + y * y + z * z);
}

Vec3& Vec3::normalize()
{
    float length = this->length();

    x /= length;
    y /= length;
    z /= length;

    return *this;
}

Vec3& Vec3::invert()
{
    x = -x;
    y = -y;
    z = -z;

    return *this;
}

void Vec3::coordinate_system(Vec3* v2, Vec3* v3) const
{
    if (std::abs(x) > std::abs(y))
        *v2 = Vec3(-z, 0, x) / std::sqrt(x * x + z * z);
    else
        *v2 = Vec3(0, z, -y) / std::sqrt(y * y + z * z);

    *v3 = this->cross(*v2);
}

Vec3 operator*(float scalar, const Vec3& vector)
{
    return vector * scalar;
}

Vec3 operator/(float scalar, const Vec3& vector)
{
    return vector / scalar;
}

std::ostream& operator<<(std::ostream& str, const Vec3& v)
{
    str << "(x: " << v.x << ", y: " << v.y << ", z: " << v.z << ")";
    return str;
}
