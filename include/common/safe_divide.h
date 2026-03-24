#pragma once

template <typename T>
T safeDivide(T numerator, T denominator)
{
    if (denominator == static_cast<T>(0))
    {
        return static_cast<T>(0);
    }

    return numerator / denominator;
}
