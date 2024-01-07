#pragma once

#include "Types.h"
#include "Containers.h"
#include "Macro.h"

#include <optional>
namespace XHuang
{
    template <typename T>
    using Optional = std::optional<T>;
    template <typename T>
    using Handle = std::shared_ptr<T>;
    template <typename T>
    using Ref = std::weak_ptr<T>;
}  // namespace XHuang

namespace XHuang
{
    template <typename T, typename... Args>
    auto MakeHandle(Args&&... args) -> decltype(std::make_shared<T>(std::forward<Args>(args)...))
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
} // namespace XHuang
