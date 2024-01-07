#include "VulkanSurface.h"

namespace XHuang
{
VulkanSurface::~VulkanSurface()
{

}

void VulkanSurface::Initialize(VkInstance instance, WindowHandle window)
{
    mInstance = instance;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    if (glfwCreateWindowSurface(instance, window->GetGLFWwindow(), nullptr, &surface) != VK_SUCCESS) {
        // log
        return;
    } else {
        mSurface = surface;
        mWindow = window;
    }
}

void VulkanSurface::Reset()
{
    if (mSurface.has_value()) {
        vkDestroySurfaceKHR(mInstance.value(), mSurface.value(), nullptr);
        mSurface.reset();
    }
}

} // namespace XHuang