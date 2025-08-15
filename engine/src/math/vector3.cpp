#include "engine/math/vector3.h"

#include <cmath>

namespace Engine::Math
{
    Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3::Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {}
    Vector3::Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    Vector3 Vector3::operator+(const Vector3 &rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
    Vector3 &Vector3::operator+=(const Vector3 &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vector3 Vector3::operator-(const Vector3 &rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
    Vector3 &Vector3::operator-=(const Vector3 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vector3 Vector3::operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
    Vector3 &Vector3::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3 Vector3::operator/(float scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }
    Vector3 &Vector3::operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    Vector3::operator glm::vec3() const { return glm::vec3(x, y, z); }

    bool Vector3::operator==(const Vector3 &rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool Vector3::operator!=(const Vector3 &rhs) const { return !(*this == rhs); }

    float Vector3::length() const { return std::sqrt(x * x + y * y + z * z); }
    float Vector3::length2() const { return x * x + y * y + z * z; }

    Vector3 Vector3::normalized() const
    {
        float len = length();
        return len > 0.0f ? Vector3(x / len, y / len, z / len) : Vector3(0.0f);
    }

    float dot(const Vector3 &a, const Vector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

    Vector3 cross(const Vector3 &a, const Vector3 &b)
    {
        return Vector3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x);
    }

    Vector3 normalize(const Vector3 &v) { return v.normalized(); }

    std::ostream &operator<<(std::ostream &os, const Vector3 &v)
    {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }
}
