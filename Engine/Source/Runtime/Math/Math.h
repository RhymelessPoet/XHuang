#pragma once
#include <cmath>

namespace XHuang
{
class Math
{
public:
    template <typename T>
    static constexpr const T& Clamp(const T& value, const T& min, const T& max)
    {
        return std::min(std::max(value, min), max);
    }
};

} // namespace XHuang