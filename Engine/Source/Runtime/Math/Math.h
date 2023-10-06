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
        return std::clamp(value, min, max);
    }
};

} // namespace XHuang