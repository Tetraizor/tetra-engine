#pragma once

#include <ostream>
#include <glm/glm.hpp>

namespace Engine::Math
{
    struct Vector3
    {
        float x, y, z;

        // === Constructors ===
        Vector3();
        explicit Vector3(float scalar);
        Vector3(float x, float y, float z);

        // === Arithmetic operators ===
        Vector3 operator+(const Vector3 &rhs) const;
        Vector3 &operator+=(const Vector3 &rhs);

        Vector3 operator-(const Vector3 &rhs) const;
        Vector3 &operator-=(const Vector3 &rhs);

        Vector3 operator*(float scalar) const;
        Vector3 &operator*=(float scalar);

        Vector3 operator/(float scalar) const;
        Vector3 &operator/=(float scalar);

        // === Conversion ===
        explicit operator glm::vec3() const;

        // === Comparison ===
        bool operator==(const Vector3 &rhs) const;
        bool operator!=(const Vector3 &rhs) const;

        // === Length / normalization ===
        float length() const;
        float length2() const;
        Vector3 normalized() const;
    };

    float dot(const Vector3 &a, const Vector3 &b);
    Vector3 cross(const Vector3 &a, const Vector3 &b);
    Vector3 normalize(const Vector3 &v);

    std::ostream &operator<<(std::ostream &os, const Vector3 &v);
}
