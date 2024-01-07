#include "VulkanCommandPool.h"
#include "VulKanDevice.h"

namespace XHuang
{

VulkanCommandPool::VulkanCommandPool(VulkanDeviceHandle device, EQueueType queueType) : mDevice(std::move(device)), mQueueType(queueType)
{
    VkCommandPool commandPool = mDevice->CreateCommandPool(mQueueType);
    if (commandPool != VK_NULL_HANDLE) {
        mCommandPool = commandPool;
    } else {
        // log error
    }
}

VulkanCommandPool::~VulkanCommandPool()
{
    if (mCommandPool.has_value()) {
        mDevice->DestroyCommandPool(mCommandPool.value());
    }
}

VkCommandBuffer VulkanCommandPool::AllocateCommandBuffer(VkCommandBufferLevel level)
{
    XH_ASSERT(mCommandPool.has_value())
    return mDevice->AllocateCommandBuffer(mCommandPool.value(), level);    
}

void VulkanCommandPool::FreeCommandBuffer(VkCommandBuffer commandBuffer)
{
    mDevice->FreeCommandBuffer(mCommandPool.value(), commandBuffer);
}

} // namespace XHuang
