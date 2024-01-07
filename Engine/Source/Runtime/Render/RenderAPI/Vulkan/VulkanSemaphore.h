#pragma once
#include "VulkanForwardDeclaration.h"
#include "Base/Base.h"

namespace XHuang
{

class VulkanSemaphore
{
public:
    VulkanSemaphore(VulkanDeviceHandle device);
    ~VulkanSemaphore();
    VkSemaphore GetVkSemaphore() const;

private:
    VulkanDeviceHandle mDevice = nullptr;
    Optional<VkSemaphore> mSemaphore;
};

} // namespace XHuang
