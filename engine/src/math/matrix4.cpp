#include "engine/math/matrix4.h"

namespace Engine::Math
{
    Matrix4::Matrix4()
    {
        for (int c = 0; c < 4; ++c)
            for (int r = 0; r < 4; ++r)
                m[c][r] = (r == c) ? 1.0f : 0.0f;
    }

    // Transformation

    Matrix4 Matrix4::translate(const Vector3 &t)
    {
        Matrix4 result = identity();

        result[3][0] = t.x;
        result[3][1] = t.y;
        result[3][2] = t.z;

        return result;
    }

    Matrix4 Matrix4::scale(const Vector3 &s)
    {
        Matrix4 result;
        for (int c = 0; c < 4; ++c)
            for (int r = 0; r < 4; ++r)
                result[c][r] = 0.0f;

        result[0][0] = s.x;
        result[1][1] = s.y;
        result[2][2] = s.z;
        result[3][3] = 1.0f;

        return result;
    }

    Matrix4 Matrix4::rotate(const Quaternion &q)
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

        // Column-major assignment (each operator[] returns a column)
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

    // Operators

    Matrix4 Matrix4::operator*(const Matrix4 &rhs) const
    {
        Matrix4 result;
        for (int c = 0; c < 4; ++c)
        {
            for (int r = 0; r < 4; ++r)
            {
                float sum = 0.0f;
                for (int k = 0; k < 4; ++k)
                    sum += m[k][r] * rhs.m[c][k]; // result[c][r] = sum_k A[k][r] * B[c][k]
                result[c][r] = sum;
            }
        }
        return result;
    }

    Vector3 Matrix4::transform_point(const Vector3 &v) const
    {
        float vx = v.x, vy = v.y, vz = v.z;
        float x = m[0][0] * vx + m[1][0] * vy + m[2][0] * vz + m[3][0];
        float y = m[0][1] * vx + m[1][1] * vy + m[2][1] * vz + m[3][1];
        float z = m[0][2] * vx + m[1][2] * vy + m[2][2] * vz + m[3][2];
        float w = m[0][3] * vx + m[1][3] * vy + m[2][3] * vz + m[3][3];

        if (std::abs(w - 1.0f) > EPSILON && std::abs(w) > EPSILON)
        {
            float iw = 1.0f / w;
            return Vector3(x * iw, y * iw, z * iw);
        }
        return Vector3(x, y, z);
    }

    Vector3 Matrix4::transform_direction(const Vector3 &v) const
    {
        float vx = v.x, vy = v.y, vz = v.z;
        float x = m[0][0] * vx + m[1][0] * vy + m[2][0] * vz;
        float y = m[0][1] * vx + m[1][1] * vy + m[2][1] * vz;
        float z = m[0][2] * vx + m[1][2] * vy + m[2][2] * vz;
        return Vector3(x, y, z);
    }

    // View operations

    Matrix4 Matrix4::perspective(float fov_radians, float aspect, float z_near, float z_far)
    {
        Matrix4 result;
        for (int c = 0; c < 4; ++c)
            for (int r = 0; r < 4; ++r)
                result[c][r] = 0.0f;

        float f = 1.0f / std::tan(fov_radians * 0.5f);
        result[0][0] = f / aspect;
        result[1][1] = f;
        result[2][2] = (z_far + z_near) / (z_near - z_far);
        result[2][3] = -1.0f;
        result[3][2] = (2.0f * z_far * z_near) / (z_near - z_far);
        return result;
    }

    Matrix4 Matrix4::orthographic(float left, float right, float bottom, float top, float z_near, float z_far)
    {
        Matrix4 result;
        for (int c = 0; c < 4; ++c)
            for (int r = 0; r < 4; ++r)
                result[c][r] = 0.0f;

        result[0][0] = 2.0f / (right - left);
        result[1][1] = 2.0f / (top - bottom);
        result[2][2] = -2.0f / (z_far - z_near);
        result[3][0] = -(right + left) / (right - left);
        result[3][1] = -(top + bottom) / (top - bottom);
        result[3][2] = -(z_far + z_near) / (z_far - z_near);
        result[3][3] = 1.0f;

        return result;
    }

    Matrix4 Matrix4::look_at(const Vector3 &eye, const Vector3 &center, const Vector3 &up)
    {
        Vector3 f = normalize(center - eye);
        Vector3 s = normalize(cross(f, up));
        Vector3 u = cross(s, f);

        Matrix4 result = identity();

        // set rotation basis in columns
        result[0][0] = s.x;
        result[0][1] = s.y;
        result[0][2] = s.z;
        result[0][3] = 0.0f;
        result[1][0] = u.x;
        result[1][1] = u.y;
        result[1][2] = u.z;
        result[1][3] = 0.0f;
        result[2][0] = -f.x;
        result[2][1] = -f.y;
        result[2][2] = -f.z;
        result[2][3] = 0.0f;

        // translation (last column)
        result[3][0] = -dot(s, eye);
        result[3][1] = -dot(u, eye);
        result[3][2] = dot(f, eye);
        result[3][3] = 1.0f;

        return result;
    }

    Matrix4 Matrix4::from_trs(const Vector3 &translation, const Quaternion &rotation, const Vector3 &scaling)
    {
        Matrix4 T = translate(translation);
        Matrix4 R = rotate(rotation);
        Matrix4 S = scale(scaling);
        return T * R * S;
    }

    Matrix4 Matrix4::inverse_affine() const
    {
        // Extract rotation/scale 3x3 (columns 0-2)
        Matrix4 inv;

        // Build 3x3
        float a00 = m[0][0], a01 = m[1][0], a02 = m[2][0];
        float a10 = m[0][1], a11 = m[1][1], a12 = m[2][1];
        float a20 = m[0][2], a21 = m[1][2], a22 = m[2][2];

        // Compute determinant
        float det = a00 * (a11 * a22 - a12 * a21) -
                    a01 * (a10 * a22 - a12 * a20) +
                    a02 * (a10 * a21 - a11 * a20);

        if (std::abs(det) < EPSILON)
        {
            // Not invertible: return identity as a safe fallback
            return Matrix4();
        }

        float invDet = 1.0f / det;

        // inverse 3x3
        float b00 = (a11 * a22 - a12 * a21) * invDet;
        float b01 = -(a01 * a22 - a02 * a21) * invDet;
        float b02 = (a01 * a12 - a02 * a11) * invDet;

        float b10 = -(a10 * a22 - a12 * a20) * invDet;
        float b11 = (a00 * a22 - a02 * a20) * invDet;
        float b12 = -(a00 * a12 - a02 * a10) * invDet;

        float b20 = (a10 * a21 - a11 * a20) * invDet;
        float b21 = -(a00 * a21 - a01 * a20) * invDet;
        float b22 = (a00 * a11 - a01 * a10) * invDet;

        // Place into inv (upper-left 3x3)
        inv[0][0] = b00;
        inv[0][1] = b10;
        inv[0][2] = b20;
        inv[0][3] = 0.0f;
        inv[1][0] = b01;
        inv[1][1] = b11;
        inv[1][2] = b21;
        inv[1][3] = 0.0f;
        inv[2][0] = b02;
        inv[2][1] = b12;
        inv[2][2] = b22;
        inv[2][3] = 0.0f;

        // Compute inverse translation = -R_inv * T
        Vector3 t(m[3][0], m[3][1], m[3][2]);
        Vector3 it;
        it.x = inv[0][0] * t.x + inv[1][0] * t.y + inv[2][0] * t.z;
        it.y = inv[0][1] * t.x + inv[1][1] * t.y + inv[2][1] * t.z;
        it.z = inv[0][2] * t.x + inv[1][2] * t.y + inv[2][2] * t.z;
        it = it * -1.0f;

        inv[3][0] = it.x;
        inv[3][1] = it.y;
        inv[3][2] = it.z;
        inv[3][3] = 1.0f;

        return inv;
    }

    Matrix4 Matrix4::transpose() const
    {
        Matrix4 t;
        for (int c = 0; c < 4; ++c)
            for (int r = 0; r < 4; ++r)
                t[c][r] = m[r][c];
        return t;
    }

    // Type conversions

    glm::mat4 Matrix4::to_glm() const
    {
        return glm::make_mat4(&m[0][0]);
    }

    Matrix4 Matrix4::from_glm(const glm::mat4 &g)
    {
        Matrix4 out;
        const float *src = &g[0][0];
        for (int c = 0; c < 4; ++c)
            for (int r = 0; r < 4; ++r)
                out[c][r] = src[c * 4 + r];
        return out;
    }

    void Matrix4::decompose(Vector3 &out_translation, Quaternion &out_rotation, Vector3 &out_scale) const
    {
        // Extract translation (last column)
        out_translation = Vector3(m[3][0], m[3][1], m[3][2]);

        // Extract columns (basis vectors)
        Vector3 col0(m[0][0], m[0][1], m[0][2]);
        Vector3 col1(m[1][0], m[1][1], m[1][2]);
        Vector3 col2(m[2][0], m[2][1], m[2][2]);

        // Scale = length of each column
        float sx = col0.length();
        float sy = col1.length();
        float sz = col2.length();
        out_scale = Vector3(sx, sy, sz);

        // Avoid division by zero
        Vector3 r0 = (sx > EPSILON) ? col0 / sx : col0;
        Vector3 r1 = (sy > EPSILON) ? col1 / sy : col1;
        Vector3 r2 = (sz > EPSILON) ? col2 / sz : col2;

        // Build rotation matrix (3x3) from normalized columns and convert to quaternion
        float R00 = r0.x, R01 = r1.x, R02 = r2.x;
        float R10 = r0.y, R11 = r1.y, R12 = r2.y;
        float R20 = r0.z, R21 = r1.z, R22 = r2.z;

        float trace = R00 + R11 + R22;
        float qw, qx, qy, qz;
        if (trace > 0.0f)
        {
            float s = std::sqrt(trace + 1.0f) * 2.0f;
            qw = 0.25f * s;
            qx = (R21 - R12) / s;
            qy = (R02 - R20) / s;
            qz = (R10 - R01) / s;
        }
        else if ((R00 > R11) && (R00 > R22))
        {
            float s = std::sqrt(1.0f + R00 - R11 - R22) * 2.0f;
            qw = (R21 - R12) / s;
            qx = 0.25f * s;
            qy = (R01 + R10) / s;
            qz = (R02 + R20) / s;
        }
        else if (R11 > R22)
        {
            float s = std::sqrt(1.0f + R11 - R00 - R22) * 2.0f;
            qw = (R02 - R20) / s;
            qx = (R01 + R10) / s;
            qy = 0.25f * s;
            qz = (R12 + R21) / s;
        }
        else
        {
            float s = std::sqrt(1.0f + R22 - R00 - R11) * 2.0f;
            qw = (R10 - R01) / s;
            qx = (R02 + R20) / s;
            qy = (R12 + R21) / s;
            qz = 0.25f * s;
        }

        out_rotation = Quaternion(qx, qy, qz, qw).normalized();
    }

}