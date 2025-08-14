#pragma once

#include <cmath>
#include <ostream>

namespace Engine::Math
{
    struct Vector3
    {
        float x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        Vector3 operator+(const Vector3 &rhs) const { return {x + rhs.x, y + rhs.y, z + rhs.z}; }
        Vector3 &operator+=(const Vector3 &rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        Vector3 operator-(const Vector3 &rhs) const { return {x - rhs.x, y - rhs.y, z - rhs.z}; }
        Vector3 &operator-=(const Vector3 &rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        Vector3 operator*(float scalar) const { return {x * scalar, y * scalar, z * scalar}; }
        Vector3 &operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        Vector3 operator/(float scalar) const { return {x / scalar, y / scalar, z / scalar}; }
        Vector3 &operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        bool operator==(const Vector3 &rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
        bool operator!=(const Vector3 &rhs) const { return !(*this == rhs); }

        float length() const { return std::sqrt(x * x + y * y + z * z); }
        float length2() const { return x * x + y * y + z * z; }

        Vector3 normalized() const
        {
            float len = length();
            return len > 0 ? Vector3(x / len, y / len, z / len) : Vector3(0.0f);
        }
    };

    inline float dot(const Vector3 &a, const Vector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
    inline Vector3 cross(const Vector3 &a, const Vector3 &b)
    {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
    }
    inline Vector3 normalize(const Vector3 &v) { return v.normalized(); }

    inline std::ostream &operator<<(std::ostream &os, const Vector3 &v)
    {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }
}