#pragma once

#include <cmath>
#include <algorithm>
#include <ostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vector3.h"
#include "quaternion.h"
#include "constants.h"

namespace Engine::Math
{
    /**
     * @brief 4x4 column-major matrix using column-vector convention (v' = M * v).
     *
     * Memory layout matches GLM's column-major layout: element at row r, column c
     * is stored as m[c][r]. operator[](int) returns a pointer to a column (not a row).
     */
    struct Matrix4
    {
        float m[4][4];

        Matrix4();

        float *operator[](int col) { return m[col]; }

        const float *operator[](int col) const { return m[col]; }

        const float *data() const { return &m[0][0]; }

        static Matrix4 identity() { return Matrix4(); }

        /**
         * @brief Equality with epsilon tolerance.
         */
        bool equals_eps(const Matrix4 &o, float eps = EPSILON) const
        {
            for (int c = 0; c < 4; ++c)
                for (int r = 0; r < 4; ++r)
                    if (std::abs(m[c][r] - o[c][r]) > eps)
                        return false;
            return true;
        }

        /**
         * @brief Multiply two matrices (this uses column-major / column-vector convention).
         * @note result = A * B
         */
        Matrix4 operator*(const Matrix4 &rhs) const;

        // === Transformations ===

        /**
         * @brief Creates a translation matrix for column-vector convention.
         * @param t translation vector
         * @return A Matrix4 representing the translation transformation.
         */
        static Matrix4 translate(const Vector3 &t);

        /**
         * @brief Creates a scaling matrix using the specified scale factors.
         *
         * Constructs and returns a 4x4 matrix that scales objects along the x, y, and z axes
         * according to the components of the provided Vector3.
         *
         * @param s A Vector3 containing the scale factors for the x, y, and z axes.
         * @return A Matrix4 representing the scaling transformation.
         */
        static Matrix4 scale(const Vector3 &s);

        /**
         * @brief Builds a rotation matrix (3x3 upper-left) from a quaternion.
         * @param q quaternion (assumed normalized or will produce rotation accordingly)
         * @return A Matrix4 representing the rotation transformation.
         */
        static Matrix4 rotate(const Quaternion &q);

        /**
         * @brief Transforms a point (x, y, z, 1). Performs perspective divide if w != 1.
         * @param v point in object space
         * @return transformed point (in same 3D space)
         */
        Vector3 transform_point(const Vector3 &v) const;

        /**
         * @brief Transforms a direction vector (w = 0). Ignores translation.
         * @param v direction vector
         */
        Vector3 transform_direction(const Vector3 &v) const;

        /**
         * @brief Compose a transformation matrix from translation, rotation and scale (T * R * S).
         * @param translation translation vector
         * @param rotation rotation quaternion
         * @param scale scale vector
         */
        static Matrix4 from_trs(const Vector3 &translation, const Quaternion &rotation, const Vector3 &scaling);

        /**
         * @brief Decomposes the matrix into translation, rotation (quaternion) and scale.
         *
         * @param out_translation receives translation
         * @param out_rotation receives rotation (as Quaternion)
         * @param out_scale receives scale
         *
         * @note This assumes the matrix is composed as T * R * S (no shear). Negative scale / reflection are not fully handled.
         */
        void decompose(Vector3 &out_translation, Quaternion &out_rotation, Vector3 &out_scale) const;

        // === View Operations ===

        /**
         * @brief Creates a perspective projection matrix (OpenGL style, right-handed).
         * @param fov_radians vertical field of view (radians)
         * @param aspect aspect ratio (width / height)
         * @param z_near near plane (positive)
         * @param z_far far plane (positive)
         */
        static Matrix4 perspective(float fov_radians, float aspect, float z_near, float z_far);

        /**
         * @brief Creates an orthographic projection matrix.
         * @param left,right,bottom,top define the view volume
         * @param znear,zfar near and far planes
         */
        static Matrix4 orthographic(float left, float right, float bottom, float top, float z_near, float z_far);

        /**
         * @brief Builds a lookAt view matrix (camera) matching GLM::lookAt (column-major).
         * @param eye camera position
         * @param center target point
         * @param up world up vector
         */
        static Matrix4 look_at(const Vector3 &eye, const Vector3 &center, const Vector3 &up);

        // === Matrix Operations ===

        /**
         * @brief Fast inverse assuming an affine TRS matrix with bottom row (0,0,0,1).
         * @note If the matrix contains projection or non-affine terms, use a full inverse routine.
         */
        Matrix4 inverse_affine() const;

        Matrix4 transpose() const;

        // === Type Conversions ===

        /**
         * @brief Converts to glm::mat4 with matching column-major layout.
         */
        glm::mat4 to_glm() const;

        /**
         * @brief Create Matrix4 from glm::mat4
         */
        static Matrix4 from_glm(const glm::mat4 &g);

        /**
         * @brief Create a matrix from a quaternion (convenience wrapper).
         */
        static Matrix4 from_quaternion(const Quaternion &q)
        {
            return rotate(q);
        }
    };

    inline std::ostream &operator<<(std::ostream &os, const Matrix4 &mat)
    {
        for (int r = 0; r < 4; ++r)
        {
            os << "| ";
            for (int c = 0; c < 4; ++c)
                os << mat.m[c][r] << " ";
            os << "|\n";
        }
        return os;
    }
}
