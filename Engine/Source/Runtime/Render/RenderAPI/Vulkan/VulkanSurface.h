#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Base/Base.h"
#include "VulkanForwardDeclaration.h"
#include "Window/Window.h"

namespace XHuang
{

class VulkanSurface
{
public:
    VulkanSurface() = default;
    ~VulkanSurface();
    bool IsComplete() const { return mSurface.has_value(); }
    void Initialize(VkInstance instance, WindowHandle window);
    void Reset();
    const WindowHandle& GetWindow() const { return mWindow; }
    VkSurfaceKHR GetVkSurface() const { return mSurface.has_value() ? mSurface.value() : VK_NULL_HANDLE; }

private:
    Optional<VkInstance>   mInstance;
    Optional<VkSurfaceKHR> mSurface;
    WindowHandle           mWindow = nullptr;
};
    
} // namespace XHuang
