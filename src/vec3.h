#ifndef VEC3_H
#define VEC3_H

#include <iostream>

class Vec3 {
public:
  union {
    struct {
      float x, y, z;
    };
    float axis[3];
  };
  Vec3() = default;
  Vec3(float v);
  Vec3(float x, float y, float z);

  operator bool() const;

  bool operator==(const Vec3 &other) const;
  Vec3 operator+(const Vec3 &other) const;
  void operator+=(const Vec3 &other);
  Vec3 operator-(const Vec3 &other) const;
  Vec3 operator-() const;
  void operator-=(const Vec3 &other);
  Vec3 operator*(float scalar) const;
  Vec3 operator*(const Vec3 &other) const;
  Vec3 operator/(float scalar) const;
  Vec3 operator/(const Vec3 &other) const;
  void operator/=(float scalar);
  float operator[](int i) const;

  Vec3 cross(const Vec3 &other) const;

  float dot(Vec3 other) const;
  float length() const;

  Vec3 &normalize();
  Vec3 &invert();

  void coordinate_system(Vec3 *v2, Vec3 *v3) const;
};

Vec3 operator*(float scalar, const Vec3 &vector);
Vec3 operator/(float scalar, const Vec3 &vector);

std::ostream &operator<<(std::ostream &str, const Vec3 &v);

#endif // VEC3_H
