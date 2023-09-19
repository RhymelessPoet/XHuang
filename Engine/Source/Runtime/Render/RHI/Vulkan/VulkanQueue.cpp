#include "VulkanQueue.h"

namespace XHuang
{
    VulkanQueue::~VulkanQueue()
    {
    }

    void VulkanQueue::Initialize(UInt32 queueFamilyIndex, UInt32 queueIndex)
    {
        ASSERT(mParentDevice.has_value())
        VkQueue vkQueue;
        vkGetDeviceQueue(mParentDevice.value(), queueFamilyIndex, queueIndex, &vkQueue);
        if (vkQueue == VK_NULL_HANDLE) {
            return;
        }
        mVulkanQueue = vkQueue;
    }

} // namespace XHuang