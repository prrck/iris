#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include "vec3.h"

std::string vec3_to_str(const Vec3& v)
{
    std::ostringstream os;
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os.str();
}

// Demonstrate some basic assertions.
TEST(Vec3, VecEqualsVec)
{
    Vec3 a(1.0, 1.0, 1.0);
    Vec3 b(1, 1, 1);
    Vec3 c(0, 0, 0);

    EXPECT_EQ(b, a) << "Vectors a and b differ " << vec3_to_str(a) << " != " << vec3_to_str(b);
    EXPECT_NE(a, c) << "Vectors a and c are equal " << vec3_to_str(a) << " == " << vec3_to_str(b);
}

TEST(Vec3, VecAddVec)
{
    Vec3 a(1.0, 1.0, 1.0);
    Vec3 b(2.0, 2.0, 2.0);
    Vec3 c(3.0, 3.0, 3.0);

    Vec3 sum = a + b;
    EXPECT_EQ(sum, c) << "Vectors c and sum differ after sum " << vec3_to_str(a) << " != " << vec3_to_str(b);
}

TEST(Vec3, VecAddVecNegative)
{
    Vec3 a(1.0, 2.0, 3.0);
    Vec3 b(-2.0, -2.0, -2.0);
    Vec3 c(-1.0, 0.0, 1.0);

    Vec3 sum = a + b;
    EXPECT_EQ(sum, c) << "Vectors c and sum differ after sum " << vec3_to_str(a) << " != " << vec3_to_str(b);
}

TEST(Vec3, VecIncrementVec)
{
    Vec3 a(1.0, 2.0, 3.0);
    Vec3 b(2.0, 2.0, 2.0);
    Vec3 c(3.0, 4.0, 5.0);

    a += b;
    EXPECT_EQ(a, c) << "Vectors a and c differ after sum " << vec3_to_str(a) << " != " << vec3_to_str(b);
}

TEST(Vec3, VecIncrementVecNegative)
{
    Vec3 a(1.0, 2.0, 3.0);
    Vec3 b(-2.0, -2.0, -2.0);
    Vec3 c(-1.0, 0.0, 1.0);

    a += b;
    EXPECT_EQ(a, c) << "Vectors a and c differ after sum " << vec3_to_str(a) << " != " << vec3_to_str(b);
}

TEST(Vec3, VecSubVec)
{
    Vec3 a(1.0, 2.0, 3.0);
    Vec3 b(2.0, 2.0, 2.0);
    Vec3 c(-1.0, 0.0, 1.0);

    Vec3 sub = a - b;
    EXPECT_EQ(sub, c) << "Vectors c and sub differ after sub " << vec3_to_str(c) << " != " << vec3_to_str(sub);
}

TEST(Vec3, VecSubVecNegative)
{
    Vec3 a(1.0, 1.0, 1.0);
    Vec3 b(-1.0, 1.0, 1.0);
    Vec3 c(2.0, 0.0, 0.0);

    Vec3 sub = a - b;
    EXPECT_EQ(sub, c) << "Vectors c and sub differ after sub " << vec3_to_str(c) << " != " << vec3_to_str(sub);
}

TEST(Vec3, VecSubUnary)
{
    Vec3 a(1.0, 1.0, 1.0);
    Vec3 c(-1.0, -1.0, -1.0);

    Vec3 inverse = -a;
    EXPECT_EQ(inverse, c) << "Vectors c and sub differ after inversion " << vec3_to_str(c) << " != " << vec3_to_str(inverse);
}

TEST(Vec3, VecDecrementVecNegative)
{
    Vec3 a(1.0, 2.0, 3.0);
    Vec3 b(2.0, 2.0, 2.0);
    Vec3 c(-1.0, 0.0, 1.0);

    a -= b;
    EXPECT_EQ(a, c) << "Vectors a and c differ after sub " << vec3_to_str(a) << " != " << vec3_to_str(b);
}

TEST(Vec3, VecdivVec)
{
    Vec3 a(1.0, 2.0, 3.0);
    Vec3 b(2.0, 2.0, 2.0);
    Vec3 c(0.5, 1.0, 1.5);

    Vec3 division = a / b;
    EXPECT_EQ(division, c) << "Vectors c and division differ after div " << vec3_to_str(a) << " != " << vec3_to_str(division);
}

TEST(Vec3, Vecdivfloat)
{
    Vec3 a(1.0, 2.0, 3.0);
    Vec3 c(0.5, 1.0, 1.5);
    float s = 2.0;

    Vec3 division = a / s;
    EXPECT_EQ(division, c) << "Vectors c and division differ after div " << vec3_to_str(a) << " != " << vec3_to_str(division);

    Vec3 division2 = s / a;
    EXPECT_EQ(division2, c) << "Vectors c and division2 differ after div " << vec3_to_str(a) << " != " << vec3_to_str(division2);
}

TEST(Vec3, CrossAxis)
{
    Vec3 a(1.0, 0.0, 0.0);
    Vec3 b(0.0, 1.0, 0.0);
    Vec3 c(0.0, 0.0, 1.0);

    Vec3 crossProduct = a.cross(b);
    EXPECT_EQ(crossProduct, c) << "Vectors c and crossProduct differ after cross " << vec3_to_str(crossProduct) << " != " << vec3_to_str(c);
}

TEST(Vec3, CrossAxisNegative)
{
    Vec3 a(-1.0, 0.0, 0.0);
    Vec3 b(0.0, -1.0, 0.0);
    Vec3 c(0.0, 0.0, 1.0);

    Vec3 crossProduct = a.cross(b);
    EXPECT_EQ(crossProduct, c) << "Vectors c and crossProduct differ after cross " << vec3_to_str(crossProduct) << " != " << vec3_to_str(c);
}
