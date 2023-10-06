#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Base/Base.h"
#include "VkForwardDeclaration.h"
#include "Window/Window.h"

namespace XHuang
{

class VulkanSurface
{
public:
    VulkanSurface() = default;
    ~VulkanSurface();
    void Initialize(VkInstance instance, WindowSPtr window);
    void Reset();
    const WindowSPtr& GetWindow() const { return mWindow; }
    VkSurfaceKHR GetSurface() const { return mSurface.has_value() ? mSurface.value() : VK_NULL_HANDLE; }

private:
    Optional<VkInstance>   mInstance;
    Optional<VkSurfaceKHR> mSurface;
    WindowSPtr             mWindow = nullptr;
};
    
} // namespace XHuang
