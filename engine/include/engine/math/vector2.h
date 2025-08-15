#pragma once

#include <ostream>
#include <glm/glm.hpp>

namespace Engine::Math
{
    struct Vector2
    {
        float x, y;

        // === Constructors ===
        Vector2();
        explicit Vector2(float scalar);
        Vector2(float x, float y);

        // === Simple constant helpers ===
        static Vector2 one();
        static Vector2 up();
        static Vector2 right();

        // === Arithmetic operators ===
        Vector2 operator+(const Vector2 &rhs) const;
        Vector2 &operator+=(const Vector2 &rhs);

        Vector2 operator-(const Vector2 &rhs) const;
        Vector2 &operator-=(const Vector2 &rhs);

        Vector2 operator*(float scalar) const;
        Vector2 &operator*=(float scalar);

        Vector2 operator/(float scalar) const;
        Vector2 &operator/=(float scalar);

        // === Comparison ===
        bool operator==(const Vector2 &rhs) const;
        bool operator!=(const Vector2 &rhs) const;

        // === Conversion ===
        explicit operator glm::vec2() const;

        // === Length / normalization ===
        float length() const;
        float length2() const;
        Vector2 normalized() const;
    };

    float dot(const Vector2 &a, const Vector2 &b);
    Vector2 normalize(const Vector2 &v);

    std::ostream &operator<<(std::ostream &os, const Vector2 &v);
}
