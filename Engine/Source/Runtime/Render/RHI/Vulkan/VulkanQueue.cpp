#include "VulkanQueue.h"
#include "VulkanDevice.h"

namespace XHuang
{
    VulkanQueue::~VulkanQueue()
    {
    }

    void VulkanQueue::Initialize(UInt32 queueFamilyIndex, UInt32 queueIndex)
    {
        ASSERT(mParentDevice.lock() != nullptr)
        VkQueue vkQueue;
        vkGetDeviceQueue(mParentDevice.lock()->GetVkDevice(), queueFamilyIndex, queueIndex, &vkQueue);
        if (vkQueue == VK_NULL_HANDLE) {
            return;
        }
        mVulkanQueue = vkQueue;
    }

    void VulkanQueue::Reset()
    {
        
    }

} // namespace XHuang