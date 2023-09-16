#pragma once
#include <memory>

#define CLASS_DECLARE(name) class name;
#define CLASS_SHAREDPTR_DECLARE(name) \
    class name; \
    using name##SPtr = std::shared_ptr<name>;

namespace XHuang
{
CLASS_SHAREDPTR_DECLARE(RHI)
CLASS_SHAREDPTR_DECLARE(VulkanRHI)

CLASS_SHAREDPTR_DECLARE(Module)
CLASS_SHAREDPTR_DECLARE(RenderModule)


} // namespace XHuang