#pragma once

#include "Types.h"
#include "Containers.h"
#include "Macro.h"

#include <optional>
namespace XHuang
{
    template <typename T>
    using Optional = std::optional<T>;
}  // namespace XHuang

namespace XHuang
{
    template <typename T, typename... Args>
    auto MakeShared(Args&&... args) -> decltype(std::make_shared<T>(std::forward<Args>(args)...))
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
} // namespace XHuang
