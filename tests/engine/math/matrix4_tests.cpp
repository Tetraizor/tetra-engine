#include <gtest/gtest.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "engine/math/matrix4.h"
#include "engine/math/constants.h"

using namespace Engine::Math;

TEST(Matrix4, Identity)
{
    Matrix4 I;
    glm::mat4 G(1.0f);

    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            EXPECT_NEAR(I[c][r], G[c][r], EPSILON);
}

TEST(Matrix4, Translation)
{
    Vector3 t(1, 2, 3);
    Matrix4 M = Matrix4::translate(t);
    glm::mat4 G = glm::translate(glm::mat4(1.0f), glm::vec3(1, 2, 3));

    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            EXPECT_NEAR(M[c][r], G[c][r], EPSILON);
}

TEST(Matrix4, Scaling)
{
    Vector3 s(2, 3, 4);
    Matrix4 M = Matrix4::scale(s);
    glm::mat4 G = glm::scale(glm::mat4(1.0f), glm::vec3(2, 3, 4));

    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            EXPECT_NEAR(M[c][r], G[c][r], EPSILON);
}

TEST(Matrix4, Rotation90Y)
{
    Quaternion q = Quaternion::from_euler(0, glm::half_pi<float>(), 0);
    Matrix4 M = Matrix4::rotate(q);

    glm::quat gq = glm::quat(glm::vec3(0, glm::half_pi<float>(), 0));
    glm::mat4 G = glm::mat4(gq);

    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            EXPECT_NEAR(M[c][r], G[c][r], EPSILON);
}

TEST(Matrix4, TRS_Decompose)
{
    Vector3 t(3, 4, 5);
    Quaternion r = Quaternion::from_euler(0.2f, 0.5f, -0.3f);
    Vector3 s(2, 2, 2);

    Matrix4 M = Matrix4::from_trs(t, r, s);
    glm::mat4 G = glm::translate(glm::mat4(1.0f), glm::vec3(3, 4, 5)) * glm::mat4(glm::quat(glm::vec3(0.2f, 0.5f, -0.3f))) * glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 2));

    // compare matrices directly
    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            EXPECT_NEAR(M[c][r], G[c][r], EPSILON);
}

TEST(Matrix4, InverseAffine)
{
    Vector3 t(2, 3, 4);
    Quaternion r = Quaternion::from_euler(0.1f, 0.2f, 0.3f);
    Vector3 s(1.5f, 2.0f, 0.5f);

    Matrix4 M = Matrix4::from_trs(t, r, s);
    glm::mat4 G = glm::translate(glm::mat4(1.0f), glm::vec3(2, 3, 4)) * glm::mat4(glm::quat(glm::vec3(0.1f, 0.2f, 0.3f))) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 2.0f, 0.5f));

    Matrix4 invM = M.inverse_affine();
    glm::mat4 invG = glm::inverse(G);

    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            EXPECT_NEAR(invM[c][r], invG[c][r], EPSILON);
}

TEST(Matrix4, PerspectiveMatchesGLM)
{
    float fov = glm::radians(60.0f);
    float aspect = 1.6f;
    float near = 0.1f, far = 100.0f;

    Matrix4 M = Matrix4::perspective(fov, aspect, near, far);
    glm::mat4 G = glm::perspective(fov, aspect, near, far);

    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            EXPECT_NEAR(M[c][r], G[c][r], EPSILON);
}

TEST(Matrix4, LookAtMatchesGLM)
{
    Vector3 eye(0, 0, 5);
    Vector3 center(0, 0, 0);
    Vector3 up(0, 1, 0);

    Matrix4 M = Matrix4::look_at(eye, center, up);
    glm::mat4 G = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            EXPECT_NEAR(M[c][r], G[c][r], EPSILON);
}