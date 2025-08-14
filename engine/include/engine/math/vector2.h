#pragma once

#include <cmath>
#include <ostream>

namespace Engine::Math
{
    struct Vector2
    {
        float x, y;

        Vector2() : x(0), y(0) {}
        Vector2(float scalar) : x(scalar), y(scalar) {}
        Vector2(float x, float y) : x(x), y(y) {}

        inline Vector2 one() { return Vector2(1, 1); }
        inline Vector2 up() { return Vector2(0, 1); }
        inline Vector2 right() { return Vector2(1, 0); }

        Vector2 operator+(const Vector2 &rhs) const { return {x + rhs.x, y + rhs.y}; }
        Vector2 &operator+=(const Vector2 &rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        Vector2 operator-(const Vector2 &rhs) const { return {x - rhs.x, y - rhs.y}; }
        Vector2 &operator-=(const Vector2 &rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        Vector2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
        Vector2 &operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vector2 operator/(float scalar) const { return {x / scalar, y / scalar}; }
        Vector2 &operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        bool operator==(const Vector2 &rhs) const { return x == rhs.x && y == rhs.y; }
        bool operator!=(const Vector2 &rhs) const { return !(*this == rhs); }

        float length() const { return std::sqrt(x * x + y * y); }
        float length2() const { return x * x + y * y; }

        Vector2 normalized() const
        {
            float len = length();
            return len > 0 ? Vector2(x / len, y / len) : Vector2();
        }

        inline float dot(const Vector2 &a, const Vector2 &b) { return a.x * b.x + a.y * b.y; }
        inline Vector2 normalize(const Vector2 &v) { return v.normalized(); }

        friend std::ostream &operator<<(std::ostream &os, const Vector2 &v)
        {
            return os << "(" << v.x << ", " << v.y << ")";
        }
    };
}