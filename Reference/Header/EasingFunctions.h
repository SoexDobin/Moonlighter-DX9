#pragma once
#include <cmath>

/// <summary>
/// Easing Funcion의 매개변수 t는 언제나 0 ~ 1 사이
/// </summary>
namespace Ease
{
    inline float Linear(float t)
    {
        return t;
    }

    inline float InQuad(float t)
    {
        return t * t;
    }

    inline float OutQuad(float t)
    {
        return t * (2 - t);
    }

    inline float InOutQuad(float t)
    {
        return t < 0.5f ? 2 * t * t : 1 - powf(-2 * t + 2, 2) / 2;
    }

    inline float InCubic(float t)
    {
        return t * t * t;
    }

    inline float OutCubic(float t)
    {
        return 1 - powf(1 - t, 3);
    }

    inline float InOutCubic(float t)
    {
        return t < 0.5f ? 4 * t * t * t : 1 - powf(-2 * t + 2, 3) / 2;
    }

    inline float OutBounce(float t)
    {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;

        if (t < 1 / d1)
            return n1 * t * t;
        else if (t < 2 / d1)
            return n1 * (t -= 1.5f / d1) * t + 0.75f;
        else if (t < 2.5 / d1)
            return n1 * (t -= 2.25f / d1) * t + 0.9375f;
        else
            return n1 * (t -= 2.625f / d1) * t + 0.984375f;
    }

    inline float OutQuint(float t)
    {
        return 1 - pow(1 - t, 5);
    }

}
