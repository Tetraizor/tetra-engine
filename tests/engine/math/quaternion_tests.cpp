#include <gtest/gtest.h>
#include "engine/math/quaternion.h"
#include "engine/math/vector3.h"
#include "engine/math/constants.h"

using namespace Engine::Math;

TEST(QuaternionTest, DefaultConstructor)
{
    Quaternion q;
    EXPECT_FLOAT_EQ(q.x, 0.0f);
    EXPECT_FLOAT_EQ(q.y, 0.0f);
    EXPECT_FLOAT_EQ(q.z, 0.0f);
    EXPECT_FLOAT_EQ(q.w, 1.0f); // Common default identity assumption
}

TEST(QuaternionTest, ParameterConstructor)
{
    Quaternion q(1, 2, 3, 4);
    EXPECT_FLOAT_EQ(q.x, 1);
    EXPECT_FLOAT_EQ(q.y, 2);
    EXPECT_FLOAT_EQ(q.z, 3);
    EXPECT_FLOAT_EQ(q.w, 4);
}

TEST(QuaternionTest, LengthAndSquared)
{
    Quaternion q(1, 2, 3, 4);
    EXPECT_FLOAT_EQ(q.length_squared(), 1 + 4 + 9 + 16);
    EXPECT_FLOAT_EQ(q.length(), std::sqrt(30.0f));
}

TEST(QuaternionTest, IsNormalized)
{
    Quaternion q = Quaternion::identity();
    EXPECT_TRUE(q.is_normalized());

    Quaternion nonNorm(0, 0, 0, 2);
    EXPECT_FALSE(nonNorm.is_normalized());
}

TEST(QuaternionTest, Identity)
{
    Quaternion id = Quaternion::identity();
    EXPECT_FLOAT_EQ(id.x, 0);
    EXPECT_FLOAT_EQ(id.y, 0);
    EXPECT_FLOAT_EQ(id.z, 0);
    EXPECT_FLOAT_EQ(id.w, 1);
}

TEST(QuaternionTest, Normalize)
{
    Quaternion q(0, 3, 0, 4); // length = 5
    Quaternion n = q.normalized();
    EXPECT_NEAR(n.length(), 1.0f, EPSILON);
}

TEST(QuaternionTest, Conjugate)
{
    Quaternion q(1, 2, 3, 4);
    Quaternion c = q.conjugate();
    EXPECT_FLOAT_EQ(c.x, -1);
    EXPECT_FLOAT_EQ(c.y, -2);
    EXPECT_FLOAT_EQ(c.z, -3);
    EXPECT_FLOAT_EQ(c.w, 4);
}

TEST(QuaternionTest, Inverse)
{
    Quaternion q = Quaternion::from_axis_angle(Vector3(0, 1, 0), PI / 4);
    Quaternion inv = q.inverse();
    Quaternion prod = q * inv;
    EXPECT_NEAR(prod.x, 0, EPSILON);
    EXPECT_NEAR(prod.y, 0, EPSILON);
    EXPECT_NEAR(prod.z, 0, EPSILON);
    EXPECT_NEAR(prod.w, 1, EPSILON);
}

TEST(QuaternionTest, MultiplyQuaternion)
{
    Quaternion q1 = Quaternion::from_axis_angle(Vector3(0, 1, 0), PI / 2);
    Quaternion q2 = Quaternion::from_axis_angle(Vector3(1, 0, 0), PI / 2);
    Quaternion result = q1 * q2;
    EXPECT_NEAR(result.length(), 1.0f, EPSILON);
}

TEST(QuaternionTest, MultiplyVector)
{
    Quaternion q = Quaternion::from_axis_angle(Vector3(0, 1, 0), PI / 2); // 90° Y rotation
    Vector3 v(1, 0, 0);
    Vector3 r = q * v;
    EXPECT_NEAR(r.x, 0.0f, EPSILON);
    EXPECT_NEAR(r.y, 0.0f, EPSILON);
    EXPECT_NEAR(r.z, -1.0f, EPSILON);
}

TEST(QuaternionTest, SLERP_Ends)
{
    Quaternion a = Quaternion::identity();
    Quaternion b = Quaternion::from_axis_angle(Vector3(0, 1, 0), PI / 2);

    Quaternion s0 = Quaternion::slerp(a, b, 0.0f);
    EXPECT_TRUE(s0 == a);

    Quaternion s1 = Quaternion::slerp(a, b, 1.0f);
    EXPECT_TRUE(s1 == b);
}

TEST(QuaternionTest, EulerConversionVectorCheck)
{
    Vector3 v1(1, 0, 0);
    Quaternion q = Quaternion::from_euler(0, PI / 2, 0);
    Vector3 result = q * v1;

    glm::vec3 glmv1(1, 0, 0);
    glm::quat glmq(glm::vec3(0, PI / 2, 0));
    glm::vec3 glmres = glmq * glmv1;

    EXPECT_NEAR(result.x, glmres.x, EPSILON);
    EXPECT_NEAR(result.y, glmres.y, EPSILON);
    EXPECT_NEAR(result.z, glmres.z, EPSILON);
}

TEST(QuaternionTest, AxisAngleConversion)
{
    Vector3 axis(0, 1, 0);
    float angle = PI / 3; // 60°

    Quaternion q = Quaternion::from_axis_angle(axis, angle);

    Vector3 outAxis;
    float outAngle;
    q.to_axis_angle(outAxis, outAngle);

    EXPECT_NEAR(outAxis.x, axis.x, EPSILON);
    EXPECT_NEAR(outAxis.y, axis.y, EPSILON);
    EXPECT_NEAR(outAxis.z, axis.z, EPSILON);
    EXPECT_NEAR(outAngle, angle, EPSILON);
}

TEST(QuaternionTest, DotProduct)
{
    Quaternion a(1, 0, 0, 0);
    Quaternion b(0, 1, 0, 0);
    EXPECT_FLOAT_EQ(dot(a, b), 0.0f);
}

TEST(QuaternionTest, NormalizeFunction)
{
    Quaternion q(0, 3, 0, 4); // length = 5
    Quaternion n = normalize(q);
    EXPECT_NEAR(n.length(), 1.0f, EPSILON);
}
