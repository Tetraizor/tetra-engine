#include <gtest/gtest.h>

#include "engine/math/vector3.h"

using namespace Engine::Math;

TEST(Vector3Test, DefaultConstructor)
{
    Vector3 v;
    EXPECT_FLOAT_EQ(v.x, 0.0f);
    EXPECT_FLOAT_EQ(v.y, 0.0f);
    EXPECT_FLOAT_EQ(v.z, 0.0f);
}

TEST(Vector3Test, ScalarConstructor)
{
    Vector3 v(5.0f);
    EXPECT_FLOAT_EQ(v.x, 5.0f);
    EXPECT_FLOAT_EQ(v.y, 5.0f);
    EXPECT_FLOAT_EQ(v.z, 5.0f);
}

TEST(Vector3Test, XYZConstructor)
{
    Vector3 v(1.0f, 2.0f, 3.0f);
    EXPECT_FLOAT_EQ(v.x, 1.0f);
    EXPECT_FLOAT_EQ(v.y, 2.0f);
    EXPECT_FLOAT_EQ(v.z, 3.0f);
}

TEST(Vector3Test, AdditionOperator)
{
    Vector3 a(1, 2, 3);
    Vector3 b(4, 5, 6);
    Vector3 c = a + b;
    EXPECT_FLOAT_EQ(c.x, 5);
    EXPECT_FLOAT_EQ(c.y, 7);
    EXPECT_FLOAT_EQ(c.z, 9);
}

TEST(Vector3Test, AdditionAssignment)
{
    Vector3 a(1, 2, 3);
    a += Vector3(4, 5, 6);
    EXPECT_FLOAT_EQ(a.x, 5);
    EXPECT_FLOAT_EQ(a.y, 7);
    EXPECT_FLOAT_EQ(a.z, 9);
}

TEST(Vector3Test, SubtractionOperator)
{
    Vector3 a(5, 7, 9);
    Vector3 b(1, 2, 3);
    Vector3 c = a - b;
    EXPECT_FLOAT_EQ(c.x, 4);
    EXPECT_FLOAT_EQ(c.y, 5);
    EXPECT_FLOAT_EQ(c.z, 6);
}

TEST(Vector3Test, SubtractionAssignment)
{
    Vector3 a(5, 7, 9);
    a -= Vector3(1, 2, 3);
    EXPECT_FLOAT_EQ(a.x, 4);
    EXPECT_FLOAT_EQ(a.y, 5);
    EXPECT_FLOAT_EQ(a.z, 6);
}

TEST(Vector3Test, MultiplyByScalar)
{
    Vector3 a(1, 2, 3);
    Vector3 b = a * 2.0f;
    EXPECT_FLOAT_EQ(b.x, 2);
    EXPECT_FLOAT_EQ(b.y, 4);
    EXPECT_FLOAT_EQ(b.z, 6);
}

TEST(Vector3Test, MultiplyAssignment)
{
    Vector3 a(1, 2, 3);
    a *= 3.0f;
    EXPECT_FLOAT_EQ(a.x, 3);
    EXPECT_FLOAT_EQ(a.y, 6);
    EXPECT_FLOAT_EQ(a.z, 9);
}

TEST(Vector3Test, DivideByScalar)
{
    Vector3 a(2, 4, 6);
    Vector3 b = a / 2.0f;
    EXPECT_FLOAT_EQ(b.x, 1);
    EXPECT_FLOAT_EQ(b.y, 2);
    EXPECT_FLOAT_EQ(b.z, 3);
}

TEST(Vector3Test, DivideAssignment)
{
    Vector3 a(2, 4, 6);
    a /= 2.0f;
    EXPECT_FLOAT_EQ(a.x, 1);
    EXPECT_FLOAT_EQ(a.y, 2);
    EXPECT_FLOAT_EQ(a.z, 3);
}

TEST(Vector3Test, EqualityOperator)
{
    Vector3 a(1, 2, 3);
    Vector3 b(1, 2, 3);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(Vector3Test, InequalityOperator)
{
    Vector3 a(1, 2, 3);
    Vector3 b(3, 2, 1);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

TEST(Vector3Test, Length)
{
    Vector3 v(3, 4, 0);
    EXPECT_FLOAT_EQ(v.length(), 5.0f);
}

TEST(Vector3Test, LengthSquared)
{
    Vector3 v(3, 4, 0);
    EXPECT_FLOAT_EQ(v.length2(), 25.0f);
}

TEST(Vector3Test, NormalizedVector)
{
    Vector3 v(3, 0, 4);
    Vector3 n = v.normalized();
    float len = std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    EXPECT_NEAR(len, 1.0f, 1e-6f);
}

TEST(Vector3Test, DotProduct)
{
    Vector3 a(1, 2, 3);
    Vector3 b(4, 5, 6);
    EXPECT_FLOAT_EQ(dot(a, b), 32.0f);
}

TEST(Vector3Test, CrossProduct)
{
    Vector3 a(1, 0, 0);
    Vector3 b(0, 1, 0);
    Vector3 c = cross(a, b);
    EXPECT_FLOAT_EQ(c.x, 0);
    EXPECT_FLOAT_EQ(c.y, 0);
    EXPECT_FLOAT_EQ(c.z, 1);
}

TEST(Vector3Test, NormalizeFunction)
{
    Vector3 v(0, 3, 4);
    Vector3 n = normalize(v);
    float len = std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    EXPECT_NEAR(len, 1.0f, 1e-6f);
}
