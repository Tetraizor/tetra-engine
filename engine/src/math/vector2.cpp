#include "engine/math/vector2.h"

#include <cmath>

namespace Engine::Math
{
    Vector2::Vector2() : x(0.0f), y(0.0f) {}
    Vector2::Vector2(float scalar) : x(scalar), y(scalar) {}
    Vector2::Vector2(float x_, float y_) : x(x_), y(y_) {}

    Vector2 Vector2::one() { return Vector2(1.0f, 1.0f); }
    Vector2 Vector2::up() { return Vector2(0.0f, 1.0f); }
    Vector2 Vector2::right() { return Vector2(1.0f, 0.0f); }

    Vector2 Vector2::operator+(const Vector2 &rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
    Vector2 &Vector2::operator+=(const Vector2 &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2 Vector2::operator-(const Vector2 &rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
    Vector2 &Vector2::operator-=(const Vector2 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vector2 Vector2::operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
    Vector2 &Vector2::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2 Vector2::operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }
    Vector2 &Vector2::operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    bool Vector2::operator==(const Vector2 &rhs) const { return x == rhs.x && y == rhs.y; }
    bool Vector2::operator!=(const Vector2 &rhs) const { return !(*this == rhs); }

    Vector2::operator glm::vec2() const { return glm::vec2(x, y); }

    float Vector2::length() const { return std::sqrt(x * x + y * y); }
    float Vector2::length2() const { return x * x + y * y; }

    Vector2 Vector2::normalized() const
    {
        float len = length();
        return len > 0.0f ? Vector2(x / len, y / len) : Vector2(0.0f, 0.0f);
    }

    float dot(const Vector2 &a, const Vector2 &b) { return a.x * b.x + a.y * b.y; }
    Vector2 normalize(const Vector2 &v) { return v.normalized(); }

    std::ostream &operator<<(std::ostream &os, const Vector2 &v)
    {
        return os << "(" << v.x << ", " << v.y << ")";
    }
}
