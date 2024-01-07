#pragma once
#include "VulkanForwardDeclaration.h"
#include "Render/RenderAPI/RenderAPI_Definition.h"
#include "Base/Base.h"

namespace XHuang
{
    
class VulkanCommandPool
{
public:
    VulkanCommandPool(VulkanDeviceHandle device, EQueueType queueType);
    ~VulkanCommandPool();
    VkCommandBuffer AllocateCommandBuffer(VkCommandBufferLevel level);
    void FreeCommandBuffer(VkCommandBuffer commandBuffer);

private:
    VulkanDeviceHandle mDevice = nullptr;
    EQueueType mQueueType = EQueueType::UnDefine;
    Optional<VkCommandPool> mCommandPool;
};


} // namespace XHuang
