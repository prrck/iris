#include <gtest/gtest.h>

#include "bounds3.h"
#include "vec3.h"

TEST(Bounds3, SurfaceAreaIdentity)
{
    Vec3 v0(0);
    Vec3 v1(1);

    Bounds3 b;
    b.expand(v0);
    b.expand(v1);

    float area = b.surface_area();

    EXPECT_EQ(area, 6) << "Surface area of a bounding box of size 1 should be 6 (got " << area << ").";
}

TEST(Bounds3, SurfaceArea112)
{
    Vec3 v0(0);
    Vec3 v1(1, 1, 2);

    Bounds3 b;
    b.expand(v0);
    b.expand(v1);

    float area = b.surface_area();

    EXPECT_EQ(area, 10) << "Surface area of a bounding box of size (1, 1, 2) should be 10 (got " << area << ").";
}
