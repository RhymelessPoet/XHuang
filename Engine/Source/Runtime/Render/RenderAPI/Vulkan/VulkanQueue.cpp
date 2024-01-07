#include "VulkanQueue.h"
#include "VulkanDevice.h"

namespace XHuang
{

VulkanQueue::VulkanQueue(VulkanDeviceHandle device, UInt32 queueFamilyIndex, UInt32 queueIndex) : mParentDevice(std::move(device))
{
    XH_ASSERT(mParentDevice.lock() != nullptr)
    VkQueue vkQueue;
    vkGetDeviceQueue(mParentDevice.lock()->GetVkDevice(), queueFamilyIndex, queueIndex, &vkQueue);
    if (vkQueue == VK_NULL_HANDLE) {
        return;
    }
    mVulkanQueue = vkQueue;
}
VulkanQueue::~VulkanQueue()
{
}

} // namespace XHuang