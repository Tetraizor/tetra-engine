#pragma once
#include <cmath>
#include <ostream>
#include "vector3.h"
#include "quaternion.h"

namespace Engine::Math
{
    struct Matrix4
    {
        float m[4][4];

        Matrix4()
        {
            for (int r = 0; r < 4; r++)
                for (int c = 0; c < 4; c++)
                    m[r][c] = (r == c) ? 1.0f : 0.0f;
        }

        float *operator[](int row) { return m[row]; }
        const float *operator[](int row) const { return m[row]; }

        static Matrix4 identity() { return Matrix4(); }

        static Matrix4 translate(const Vector3 &t)
        {
            Matrix4 result = identity();

            result[0][3] = t.x;
            result[1][3] = t.y;
            result[2][3] = t.z;

            return result;
        }

        static Matrix4 scale(const Vector3 &s)
        {
            Matrix4 result;

            result[0][0] = s.x;
            result[1][1] = s.y;
            result[2][2] = s.z;

            return result;
        }

        static Matrix4 rotate(const Quaternion &q)
        {
            Matrix4 result = identity();

            float xx = q.x * q.x;
            float yy = q.y * q.y;
            float zz = q.z * q.z;
            float xy = q.x * q.y;
            float xz = q.x * q.z;
            float yz = q.y * q.z;
            float wx = q.w * q.x;
            float wy = q.w * q.y;
            float wz = q.w * q.z;

            result[0][0] = 1.0f - 2.0f * (yy + zz);
            result[0][1] = 2.0f * (xy + wz);
            result[0][2] = 2.0f * (xz - wy);

            result[1][0] = 2.0f * (xy - wz);
            result[1][1] = 1.0f - 2.0f * (xx + zz);
            result[1][2] = 2.0f * (yz + wx);

            result[2][0] = 2.0f * (xz + wy);
            result[2][1] = 2.0f * (yz - wx);
            result[2][2] = 1.0f - 2.0f * (xx + yy);

            return result;
        }

        Matrix4 operator*(const Matrix4 &rhs) const
        {
            Matrix4 result;
            for (int r = 0; r < 4; r++)
            {
                for (int c = 0; c < 4; c++)
                {
                    result[r][c] = m[r][0] * rhs[0][c] +
                                   m[r][1] * rhs[1][c] +
                                   m[r][2] * rhs[2][c] +
                                   m[r][3] * rhs[3][c];
                }
            }
            return result;
        }

        Vector3 operator*(const Vector3 &v) const
        {
            float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
            float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
            float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
            return Vector3(x, y, z);
        }

        static Matrix4 perspective(float fov_radians, float aspect, float near, float far)
        {
            Matrix4 result;
            float tanHalfFov = std::tan(fov_radians / 2.0f);

            for (int r = 0; r < 4; r++)
                for (int c = 0; c < 4; c++)
                    result[r][c] = 0.0f;

            result[0][0] = 1.0f / (aspect * tanHalfFov);
            result[1][1] = 1.0f / tanHalfFov;
            result[2][2] = -(far + near) / (far - near);
            result[2][3] = -(2.0f * far * near) / (far - near);
            result[3][2] = -1.0f;

            return result;
        }

        static Matrix4 look_at(const Vector3 &eye, const Vector3 &center, const Vector3 &up)
        {
            Vector3 f = normalize(center - eye);
            Vector3 s = normalize(cross(f, up));
            Vector3 u = cross(s, f);

            Matrix4 result = identity();
            result[0][0] = s.x;
            result[0][1] = s.y;
            result[0][2] = s.z;

            result[1][0] = u.x;
            result[1][1] = u.y;
            result[1][2] = u.z;

            result[2][0] = -f.x;
            result[2][1] = -f.y;
            result[2][2] = -f.z;

            result[0][3] = -dot(s, eye);
            result[1][3] = -dot(u, eye);
            result[2][3] = dot(f, eye);

            return result;
        }

        static Matrix4 from_quaternion(Quaternion q)
        {
            Matrix4 result;

            float x = q.x;
            float y = q.y;
            float z = q.z;
            float w = q.w;

            float xx = x * x;
            float yy = y * y;
            float zz = z * z;
            float xy = x * y;
            float xz = x * z;
            float yz = y * z;
            float wx = w * x;
            float wy = w * y;
            float wz = w * z;

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
    };

    inline std::ostream &operator<<(std::ostream &os, const Matrix4 &mat)
    {
        for (int r = 0; r < 4; r++)
        {
            os << "| ";
            for (int c = 0; c < 4; c++)
            {
                os << mat[r][c] << " ";
            }
            os << "|\n";
        }
        return os;
    }
}
