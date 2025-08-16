#pragma once

#include <cmath>
#include <ostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "vector3.h"
#include "constants.h"

namespace Engine::Math
{
    struct Matrix4;

    struct Quaternion
    {
        float x, y, z, w;

        Quaternion();
        Quaternion(float x, float y, float z, float w);

        float length_squared() const;
        float length() const;
        bool is_normalized() const;

        static Quaternion identity();

        Quaternion normalized() const;
        Quaternion conjugate() const;
        Quaternion inverse() const;

        Quaternion operator*(const Quaternion &q) const;

        Vector3 operator*(const Vector3 &v) const;

        bool operator==(const Quaternion &rhs) const;
        bool operator!=(const Quaternion &rhs) const;

        static Quaternion slerp(const Quaternion &a, const Quaternion &b, float t);

        static Quaternion from_euler(const Vector3 &euler_radians);
        static Quaternion from_euler(float pitch, float yaw, float roll);

        Vector3 to_euler() const;

        static Quaternion from_axis_angle(const Vector3 &axis, float radians);
        void to_axis_angle(Vector3 &axis, float &angle) const;

        Matrix4 to_mat4() const;

        explicit operator glm::quat() const;
        glm::quat to_glm() const;
    };

    float dot(const Quaternion &a, const Quaternion &b);
    Quaternion normalize(const Quaternion &q);

    std::ostream &operator<<(std::ostream &os, const Quaternion &q);
}
