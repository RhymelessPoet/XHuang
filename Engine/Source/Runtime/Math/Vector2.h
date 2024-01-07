#pragma once
#include "Base/Types.h"
#include "Math.h"

namespace XHuang
{
template <typename Type>
class Vector2
{
public:
    using ValueType = Type;
    union
    {
        ValueType data[2];
        struct
        {
            ValueType X;
            ValueType Y;
        };
    };
public:
    explicit Vector2() = default;
    explicit Vector2(const ValueType& x, const ValueType& y) : X(x), Y(y), data{x, y} {}
    ValueType& operator[](UInt32 index) { return data[index]; }
    const ValueType& operator[](UInt32 index) const { return data[index]; }

};

} // namespace XHuang
#include "Vector2.ini"