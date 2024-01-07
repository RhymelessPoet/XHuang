#include "VulkanFrameBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanImageView.h"

namespace XHuang
{
VulkanFrameBuffer::VulkanFrameBuffer(VulkanRenderPassHandle renderPass, Vector<VulkanImageViewHandle> imageViews, Extent2D size, UInt32 layerCounts)
                   : mRenderPass(std::move(renderPass)), mAttachments(std::move(imageViews)), mSize(size), mLayerCount(layerCounts)
{
    UInt32 attachmentsCount = static_cast<UInt32>(mAttachments.size());
    Vector<VkImageView> attachments(attachmentsCount);
    for (UInt32 index = 0; index < attachmentsCount; ++index) {
        attachments[index] = mAttachments[index]->GetVkImageView();
    }
    VkFramebuffer frameBuffer = mRenderPass->CreateFrameBuffer(attachments, mSize.X, mSize.Y, mLayerCount);
    if (frameBuffer == VK_NULL_HANDLE) {
        // log error
    } else {
        mFrameBuffer = frameBuffer;
    }
}

VulkanFrameBuffer::~VulkanFrameBuffer()
{
    if (mFrameBuffer.has_value()) {
        mRenderPass->DestroyFrameBuffer(mFrameBuffer.value());
    }
}

VkFramebuffer VulkanFrameBuffer::GetVkFramebuffer() const
{
    XH_ASSERT(mFrameBuffer.has_value())
    return mFrameBuffer.value();
}

} // namespace XHuang