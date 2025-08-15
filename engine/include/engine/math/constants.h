#pragma once

#include <cfloat> // For FLT_EPSILON

namespace Engine::Math
{
    // --- Pi and variants ---
    constexpr float PI = 3.14159265358979323846f;
    constexpr float HALF_PI = PI * 0.5f;
    constexpr float TWO_PI = PI * 2.0f;

    // --- Degree <-> Radian conversions ---
    constexpr float DEG2RAD = PI / 180.0f;
    constexpr float RAD2DEG = 180.0f / PI;

    // --- Floating point tolerances ---
    constexpr float EPSILON = 1e-6f;
    constexpr float FLOAT_EPS = FLT_EPSILON;
}

namespace Engine::Physics
{
    constexpr float GRAVITY = 9.80665f;
}
