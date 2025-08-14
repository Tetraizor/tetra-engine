#pragma once
#include <cmath>
#include <ostream>
#include "vector3.h"

namespace Engine::Math
{
    struct Quaternion
    {
        float x, y, z, w;

        Quaternion() : x(0), y(0), z(0), w(1) {}
        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        static Quaternion identity() { return Quaternion(0, 0, 0, 1); }

        static Quaternion from_axis_angle(const Vector3 &axis, float radians)
        {
            float half_angle = radians * 0.5f;
            float s = std::sin(half_angle);

            return Quaternion(axis.x * s, axis.y * s, axis.z * s, std::cos(half_angle));
        }

        static Quaternion from_euler(float pitch, float yaw, float roll)
        {
            float cy = std::cos(yaw * 0.5f);
            float sy = std::sin(yaw * 0.5f);
            float cp = std::cos(pitch * 0.5f);
            float sp = std::sin(pitch * 0.5f);
            float cr = std::cos(roll * 0.5f);
            float sr = std::sin(roll * 0.5f);

            return Quaternion(
                sr * cp * cy - cr * sp * sy, // x
                cr * sp * cy + sr * cp * sy, // y
                cr * cp * sy - sr * sp * cy, // z
                cr * cp * cy + sr * sp * sy  // w
            );
        }

        Quaternion normalized() const
        {
            float len = std::sqrt(x * x + y * y + z * z + w * w);
            if (len > 0.0f)
            {
                float inv = 1.0f / len;
                return Quaternion(x * inv, y * inv, z * inv, w * inv);
            }
            return Quaternion();
        }

        Quaternion conjugate() const
        {
            return Quaternion(-x, -y, -z, w);
        }

        Quaternion inverse() const
        {
            float len2 = x * x + y * y + z * z + w * w;
            if (len2 > 0.0f)
            {
                float inv = 1.0f / len2;
                return Quaternion(-x * inv, -y * inv, -z * inv, w * inv);
            }
            return Quaternion();
        }

        Quaternion operator*(const Quaternion &q) const
        {
            return Quaternion(
                w * q.x + x * q.w + y * q.z - z * q.y,
                w * q.y - x * q.z + y * q.w + z * q.x,
                w * q.z + x * q.y - y * q.x + z * q.w,
                w * q.w - x * q.x - y * q.y - z * q.z);
        }

        Vector3 operator*(const Vector3 &v) const
        {
            Vector3 u(x, y, z);
            float s = w;

            Vector3 uv = cross(u, v);
            Vector3 uuv = cross(u, uv);

            uv *= (2.0f * s);
            uuv *= 2.0f;

            return v + uv + uuv;
        }

        explicit operator glm::quat() const { return glm::quat(w, x, y, z); }
        glm::quat to_glm() const { return glm::quat(w, x, y, z); }
    };

    inline float dot(const Quaternion &a, const Quaternion &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    inline Quaternion normalize(const Quaternion &q)
    {
        return q.normalized();
    }

    inline std::ostream &operator<<(std::ostream &os, const Quaternion &q)
    {
        return os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
    }
}
