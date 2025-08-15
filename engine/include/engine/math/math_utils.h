#pragma once

#include "constants.h"

namespace Engine::Math
{
    inline float degrees_to_radians(float degrees)
    {
        return degrees * DEG2RAD;
    }

    inline float radians_to_degrees(float radians)
    {
        return radians * RAD2DEG;
    }
}