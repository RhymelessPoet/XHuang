#pragma once
#include "Render/RenderAPI/GraphicsAPI.h"
#include "VulkanForwardDeclaration.h"
#include "Base/Base.h"

namespace XHuang
{
template <>
struct GraphicsCommandBuffer<Vulkan>
{
    using CommandBufferType    = VulkanCommandBuffer;
    using RenderPassType       = VulkanRenderPass;
    using GraphicsPipelineType = VulkanGraphicsPipeline;
    using FrameBufferType      = VulkanFrameBuffer;
};

class VulkanCommandBuffer : public GraphicsCommandBuffer<Vulkan>
{
public:
    VulkanCommandBuffer(VulkanCommandPoolHandle commandPool);
    ~VulkanCommandBuffer();
    VkCommandBuffer GetVkCommandBuffer() const { XH_ASSERT(mCommandBuffer.has_value()) return mCommandBuffer.value(); }

    void CommandBegin();
    void CommandBeginPass(const VulkanRenderPassHandle& passHandle, const VulkanFrameBufferHandle& framebuffer);
    void CommandBindPipeline(const VulkanGraphicsPipelineHandle& pipelineHandle);
    void CommandDraw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex, UInt32 firstInstance);
    void CommandEndPass(); 
    void CommandEnd();
    
private:
    VulkanCommandPoolRef mCommandPool;
    Optional<VkRenderPassBeginInfo> mRenderPassInfo;
    Optional<VkCommandBuffer> mCommandBuffer;
};
    
} // namespace XHuang

