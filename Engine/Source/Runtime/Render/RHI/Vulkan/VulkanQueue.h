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
    bool IsComplete() const { return mVulkanQueue.has_value(); }
    void Initialize(UInt32 queueFamilyIndex, UInt32 queueIndex);
    void Reset();

private:
    Optional<VkDevice> mParentDevice; 
    Optional<VkQueue> mVulkanQueue;
};

} // namespace XHuang