#pragma once

#include <cmath>
#include <iostream>
#include <random>

#define M_PI 3.14159265358979323846

constexpr float kInfinity = std::numeric_limits<float>::max();

inline float clamp(const float& lo, const float& hi, const float& v)
{
    return std::max(lo, std::min(hi, v));
}

inline bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0)
        return false;
    else if (discr == 0)
        x0 = x1 = -0.5f * b / a;
    else
    {
        float q = (b > 0) ? 0.5f * (-b - sqrt(discr)) : 0.5f * (-b + sqrt(discr));
        x0 = q / a;
        // X1 * X2 = c / a
        x1 = c / q;
    }
    if (x0 > x1)
        std::swap(x0, x1);
    return true;
}

// 在 global.hpp 的 MaterialType 枚举中添加新类型
enum MaterialType
{
    DIFFUSE_AND_GLOSSY,
    REFLECTION_AND_REFRACTION,
    REFLECTION,
    TRANSPARENT,    // 新增透明材质
    EMISSIVE        // 新增自发光材质
};

inline float get_random_float()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<float> dist(0.f, 1.f); // distribution in range [1, 6]

    return dist(rng);
}

inline void UpdateProgress(float progress)
{
    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}
