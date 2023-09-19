#pragma once
#include <vulkan/vulkan.h>
#include "Base/Base.h"

namespace XHuang
{
class VulkanQueue
{
public:
    VulkanQueue(VkDevice device) : mParentDevice(device) { };
    ~VulkanQueue();
    void Initialize(UInt32 queueFamilyIndex, UInt32 queueIndex);
    bool IsComplete() const { return mVulkanQueue.has_value(); }

private:
    Optional<VkDevice> mParentDevice; 
    Optional<VkQueue> mVulkanQueue;
};

} // namespace XHuang