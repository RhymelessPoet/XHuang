#pragma once
#include "VulkanForwardDeclaration.h"
#include "Base/Base.h"
#include "Render/RenderAPI/RenderAPI_Definition.h"

namespace XHuang
{
class VulkanFrameBuffer
{
public:
    VulkanFrameBuffer(VulkanRenderPassHandle renderPass, Vector<VulkanImageViewHandle> imageViews, Extent2D size, UInt32 layerCounts);
    ~VulkanFrameBuffer();
    VkFramebuffer GetVkFramebuffer() const;
    const Extent2D& GetSize() const { return mSize; }

private:
    VulkanRenderPassHandle mRenderPass;
    Optional<VkFramebuffer> mFrameBuffer;
    Extent2D mSize;
    Vector<VulkanImageViewHandle> mAttachments;
    UInt32 mLayerCount = 1u;
};

} // namespace XHuang