#pragma once
#include <vulkan/vulkan.h>
#include "Base/Base.h"
#include "VulkanForwardDeclaration.h"

namespace XHuang
{
class VulkanQueue
{
public:
    VulkanQueue(VulkanDeviceHandle device, UInt32 queueFamilyIndex, UInt32 queueIndex);
    ~VulkanQueue();
    bool IsComplete() const { return mVulkanQueue.has_value(); }
    VkQueue GetVkQueue() const { return mVulkanQueue.value(); }
 
private:
    VulkanDeviceRef   mParentDevice; 
    Optional<VkQueue> mVulkanQueue;
};

} // namespace XHuang