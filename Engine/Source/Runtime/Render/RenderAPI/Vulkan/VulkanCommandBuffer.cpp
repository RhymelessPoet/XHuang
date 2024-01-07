#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanRenderPass.h"
#include "VulkanFrameBuffer.h"
#include "VulkanGraphicsPipeline.h"

namespace XHuang
{
    
VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandPoolHandle commandPool) : mCommandPool(std::move(commandPool))
{
    VkCommandBuffer commandBuffer = mCommandPool.lock()->AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    if (commandBuffer == VK_NULL_HANDLE) {
        // log error
    } else {
        mCommandBuffer = commandBuffer;
    }
}

VulkanCommandBuffer::~VulkanCommandBuffer()
{
    if (mCommandBuffer.has_value()) {
        mCommandPool.lock()->FreeCommandBuffer(mCommandBuffer.value());
    }
}

void VulkanCommandBuffer::CommandBegin()
{
    XH_ASSERT(mCommandBuffer.has_value())
    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext            = nullptr;
    beginInfo.flags            = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(mCommandBuffer.value(), &beginInfo) != VK_SUCCESS) {
        // log error
    }
}

void VulkanCommandBuffer::CommandEnd()
{
    XH_ASSERT(mCommandBuffer.has_value())
    if (vkEndCommandBuffer(mCommandBuffer.value()) != VK_SUCCESS) {
        // log error
    }
}

void VulkanCommandBuffer::CommandBeginPass(const VulkanRenderPassHandle& passHandle, const VulkanFrameBufferHandle& framebuffer)
{
    XH_ASSERT(mCommandBuffer.has_value())
    const auto& framebufferSize = framebuffer->GetSize();
    VkClearValue clearValue{};
    clearValue.color = {{0.01f, 0.01f, 0.033f, 1.0f}};
    VkRenderPassBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.renderPass = passHandle->GetVkRenderPass();
    beginInfo.framebuffer = framebuffer->GetVkFramebuffer();
    beginInfo.renderArea.offset = { 0u, 0u };
    beginInfo.renderArea.extent = { framebufferSize.X, framebufferSize.Y };
    beginInfo.clearValueCount = 1;
    beginInfo.pClearValues = &clearValue;

    mRenderPassInfo = beginInfo;    
    vkCmdBeginRenderPass(mCommandBuffer.value(), &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandBuffer::CommandEndPass()
{
    XH_ASSERT(mCommandBuffer.has_value())
    vkCmdEndRenderPass(mCommandBuffer.value());
}

void VulkanCommandBuffer::CommandBindPipeline(const VulkanGraphicsPipelineHandle& pipelineHandle)
{
    XH_ASSERT(mCommandBuffer.has_value())
    vkCmdBindPipeline(mCommandBuffer.value(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineHandle->GetVkPipeline());
}

void VulkanCommandBuffer::CommandDraw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex, UInt32 firstInstance)
{
    XH_ASSERT(mCommandBuffer.has_value())
    vkCmdDraw(mCommandBuffer.value(), vertexCount, instanceCount, firstVertex, firstInstance);
}

} // namespace XHuang
