#pragma once
#include "Render/RenderAPI/GraphicsAPI.h"
#include "VulkanForwardDeclaration.h"
#include "Base/Base.h"

namespace XHuang
{
template <>
struct GraphicsContext<Vulkan>
{
    using CommandBufferType = VulkanCommandBuffer;
    using RenderPassType = VulkanRenderPass;
    using FrameBufferType = VulkanFrameBuffer;
};
class VulkanContext : public GraphicsContext<Vulkan>
{
public: 
    VulkanContext(VulkanDeviceHandle mDevice, VulkanSwapchainHandle swapChainHandle, VulkanCommandBufferHandle commandBuffer);
    ~VulkanContext();
    VulkanCommandBufferHandle& GetCommandBuffer() { return mCommandBuffer; }
    UInt32 GetNextFrameBufferIndex() const;
    void Synchronous();
    void Submit();
    void Swap();

private:
    mutable Optional<UInt32>  mNextImageIndex;
    VulkanDeviceHandle        mDevice;
    VulkanQueueHandle         mGraphicsQueue;
    VulkanQueueHandle         mPresentQueue;
    VulkanCommandBufferHandle mCommandBuffer;
    VulkanSwapchainHandle     mSwapChainHandle;
    VulkanSemaphoreHandle     mImageAvailableSemaphore;
    VulkanSemaphoreHandle     mRenderFinishedSemaphore;
    VulkanFenceHandle         mSynchronousFence;
};

} // namespace XHuang