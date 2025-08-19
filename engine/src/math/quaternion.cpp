#include "engine/math/quaternion.h"
#include "engine/math/matrix4.h"

#include <algorithm>

namespace Engine::Math
{
    Quaternion::Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    float Quaternion::length_squared() const { return x * x + y * y + z * z + w * w; }
    float Quaternion::length() const { return std::sqrt(length_squared()); }
    bool Quaternion::is_normalized() const { return std::abs(length_squared() - 1.0f) < (EPSILON * 10.0f); }

    Quaternion Quaternion::identity() { return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }

    Quaternion Quaternion::normalized() const
    {
        float len = length();
        if (len > EPSILON)
        {
            float inv = 1.0f / len;
            return Quaternion(x * inv, y * inv, z * inv, w * inv);
        }
        return Quaternion::identity();
    }

    Quaternion Quaternion::conjugate() const { return Quaternion(-x, -y, -z, w); }

    Quaternion Quaternion::inverse() const
    {
        float len2 = length_squared();
        if (len2 > EPSILON)
        {
            float inv = 1.0f / len2;
            return Quaternion(-x * inv, -y * inv, -z * inv, w * inv);
        }
        return Quaternion::identity();
    }

    Quaternion Quaternion::operator*(const Quaternion &q) const
    {
        return Quaternion(
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w,
            w * q.w - x * q.x - y * q.y - z * q.z);
    }

    bool Quaternion::operator==(const Quaternion &other) const
    {
        auto cmp = [&](float a, float b)
        {
            return std::fabs(a - b) <= EPSILON;
        };

        bool same =
            cmp(x, other.x) &&
            cmp(y, other.y) &&
            cmp(z, other.z) &&
            cmp(w, other.w);

        bool negated =
            cmp(x, -other.x) &&
            cmp(y, -other.y) &&
            cmp(z, -other.z) &&
            cmp(w, -other.w);

        return same || negated;
    }

    bool Quaternion::operator!=(const Quaternion &rhs) const
    {
        return !(*this == rhs);
    }

    Vector3 Quaternion::operator*(const Vector3 &v) const
    {
        Quaternion qv(v.x, v.y, v.z, 0);
        Quaternion res = (*this) * qv * this->inverse();

        return Vector3(res.x, res.y, res.z);
    }

    Quaternion Quaternion::slerp(const Quaternion &a, const Quaternion &b, float t)
    {
        if (t <= 0.0f)
            return a;
        if (t >= 1.0f)
            return b;

        float cos_theta = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        Quaternion b_copy = b;

        // if dot < 0, negate b to take the shorter arc
        if (cos_theta < 0.0f)
        {
            b_copy = Quaternion(-b.x, -b.y, -b.z, -b.w);
            cos_theta = -cos_theta;
        }

        // linear fallback for very close quaternions
        if (cos_theta > 0.9995f)
        {
            Quaternion result(
                a.x + t * (b_copy.x - a.x),
                a.y + t * (b_copy.y - a.y),
                a.z + t * (b_copy.z - a.z),
                a.w + t * (b_copy.w - a.w));
            return result.normalized();
        }

        float theta = std::acos(std::clamp(cos_theta, -1.0f, 1.0f));
        float sin_theta = std::sqrt(std::max(0.0f, 1.0f - cos_theta * cos_theta));

        if (sin_theta < EPSILON) // numerical safety
        {
            Quaternion result(
                a.x + t * (b_copy.x - a.x),
                a.y + t * (b_copy.y - a.y),
                a.z + t * (b_copy.z - a.z),
                a.w + t * (b_copy.w - a.w));
            return result.normalized();
        }

        float a_factor = std::sin((1.0f - t) * theta) / sin_theta;
        float b_factor = std::sin(t * theta) / sin_theta;

        return Quaternion(
            a.x * a_factor + b_copy.x * b_factor,
            a.y * a_factor + b_copy.y * b_factor,
            a.z * a_factor + b_copy.z * b_factor,
            a.w * a_factor + b_copy.w * b_factor);
    }

    Quaternion Quaternion::from_euler(const Vector3 &euler_radians)
    {
        return from_euler(euler_radians.x, euler_radians.y, euler_radians.z);
    }

    Quaternion Quaternion::from_euler(float pitch, float yaw, float roll)
    {
        // half angles
        float hx = pitch * 0.5f;
        float hy = yaw * 0.5f;
        float hz = roll * 0.5f;

        float cx = cos(hx), sx = sin(hx);
        float cy = cos(hy), sy = sin(hy);
        float cz = cos(hz), sz = sin(hz);

        Quaternion q;
        q.w = cx * cy * cz + sx * sy * sz;
        q.x = sx * cy * cz - cx * sy * sz;
        q.y = cx * sy * cz + sx * cy * sz;
        q.z = cx * cy * sz - sx * sy * cz;
        return q;
    }

    Vector3 Quaternion::to_euler() const
    {
        Vector3 euler;

        // Pitch (X)
        float sin_p = 2.0f * (w * x + y * z);
        float cos_p = 1.0f - 2.0f * (x * x + y * y);
        euler.x = std::atan2(sin_p, cos_p);

        // Yaw (Y)
        float sin_y = 2.0f * (w * y - z * x);
        if (std::abs(sin_y) >= 1.0f)
            euler.y = std::copysign(HALF_PI, sin_y);
        else
            euler.y = std::asin(sin_y);

        // Roll (Z)
        float sin_r = 2.0f * (w * z + x * y);
        float cos_r = 1.0f - 2.0f * (y * y + z * z);
        euler.z = std::atan2(sin_r, cos_r);

        return euler;
    }

    Quaternion Quaternion::from_axis_angle(const Vector3 &axis, float radians)
    {
        float half_angle = radians * 0.5f;
        float s = std::sin(half_angle);
        return Quaternion(axis.x * s, axis.y * s, axis.z * s, std::cos(half_angle));
    }

    void Quaternion::to_axis_angle(Vector3 &axis, float &angle) const
    {
        Quaternion q = normalized();
        angle = 2.0f * std::acos(q.w);
        float tmp = 1.0f - q.w * q.w;
        float s = std::sqrt(std::max(0.0f, tmp));
        if (s < EPSILON)
            axis = Vector3(1, 0, 0);
        else
            axis = Vector3(q.x / s, q.y / s, q.z / s);
    }

    Matrix4 Quaternion::to_mat4() const
    {
        Matrix4 result;
        float xx = x * x, yy = y * y, zz = z * z;
        float xy = x * y, xz = x * z, yz = y * z;
        float wx = w * x, wy = w * y, wz = w * z;

        result[0][0] = 1.0f - 2.0f * (yy + zz);
        result[0][1] = 2.0f * (xy + wz);
        result[0][2] = 2.0f * (xz - wy);
        result[0][3] = 0.0f;

        result[1][0] = 2.0f * (xy - wz);
        result[1][1] = 1.0f - 2.0f * (xx + zz);
        result[1][2] = 2.0f * (yz + wx);
        result[1][3] = 0.0f;

        result[2][0] = 2.0f * (xz + wy);
        result[2][1] = 2.0f * (yz - wx);
        result[2][2] = 1.0f - 2.0f * (xx + yy);
        result[2][3] = 0.0f;

        result[3][0] = 0.0f;
        result[3][1] = 0.0f;
        result[3][2] = 0.0f;
        result[3][3] = 1.0f;

        return result;
    }

    Quaternion::operator glm::quat() const { return glm::quat(w, x, y, z); }
    glm::quat Quaternion::to_glm() const { return glm::quat(w, x, y, z); }

    float dot(const Quaternion &a, const Quaternion &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    Quaternion normalize(const Quaternion &q)
    {
        return q.normalized();
    }

    std::ostream &operator<<(std::ostream &os, const Quaternion &q)
    {
        return os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
    }
}
