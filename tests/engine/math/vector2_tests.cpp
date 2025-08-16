#include <gtest/gtest.h>

#include "engine/math/vector2.h"
#include "engine/math/constants.h"

using namespace Engine::Math;

TEST(Vector2Tests, ConstructorsAndConstants)
{
    Vector2 a; // default
    EXPECT_NEAR(a.x, 0.0f, EPSILON);
    EXPECT_NEAR(a.y, 0.0f, EPSILON);

    Vector2 s(2.5f);
    EXPECT_NEAR(s.x, 2.5f, EPSILON);
    EXPECT_NEAR(s.y, 2.5f, EPSILON);

    Vector2 v(3.0f, -4.0f);
    EXPECT_NEAR(v.x, 3.0f, EPSILON);
    EXPECT_NEAR(v.y, -4.0f, EPSILON);

    Vector2 o = Vector2::one();
    EXPECT_EQ(o, Vector2(1.0f, 1.0f));
    EXPECT_EQ(Vector2::up(), Vector2(0.0f, 1.0f));
    EXPECT_EQ(Vector2::right(), Vector2(1.0f, 0.0f));
}

TEST(Vector2Tests, ArithmeticOperators)
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(3.0f, 4.0f);

    Vector2 c = a + b;
    EXPECT_EQ(c, Vector2(4.0f, 6.0f));

    a += b;
    EXPECT_EQ(a, Vector2(4.0f, 6.0f));

    Vector2 d = c - b;
    EXPECT_EQ(d, Vector2(1.0f, 2.0f));

    d -= Vector2(1.0f, 1.0f);
    EXPECT_EQ(d, Vector2(0.0f, 1.0f));

    Vector2 e = b * 2.0f;
    EXPECT_EQ(e, Vector2(6.0f, 8.0f));

    e /= 2.0f;
    EXPECT_EQ(e, b);
}

TEST(Vector2Tests, LengthAndNormalizeDot)
{
    Vector2 v(3.0f, 4.0f);
    EXPECT_NEAR(v.length(), 5.0f, EPSILON);
    EXPECT_NEAR(v.length2(), 25.0f, EPSILON);

    Vector2 n = v.normalized();
    EXPECT_NEAR(n.length(), 1.0f, 1e-5f);
    EXPECT_NEAR(n.x, 3.0f / 5.0f, 1e-5f);
    EXPECT_NEAR(n.y, 4.0f / 5.0f, 1e-5f);

    // dot product
    EXPECT_NEAR(dot(Vector2(1, 2), Vector2(3, 4)), 11.0f, EPSILON);
    EXPECT_NEAR(dot(n, n), 1.0f, 1e-5f);

    // normalize free function
    Vector2 nz = normalize(Vector2(0.0f, 0.0f));
    EXPECT_EQ(nz, Vector2(0.0f, 0.0f));
}

TEST(Vector2Tests, GLMConversion)
{
    Vector2 v(1.25f, -2.5f);
    glm::vec2 g = static_cast<glm::vec2>(v);
    EXPECT_NEAR(g.x, v.x, EPSILON);
    EXPECT_NEAR(g.y, v.y, EPSILON);
}
