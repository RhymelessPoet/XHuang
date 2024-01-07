#include "VulkanForwardDeclaration.h"
#include "Base/Base.h"
#include "Render/RenderAPI/GraphicsAPI.h"

namespace XHuang
{

template <>
struct GraphicsRenderPass<Vulkan>
{
    using GraphicsAPI_Type     = Vulkan;
    using FrameBufferType      = VulkanFrameBuffer;
    using ImageViewType        = VulkanImageView;
    using GraphicsPipelineType = VulkanGraphicsPipeline;
    using RHIType              = VulkanRHI;
    using ShaderType           = VulkanShader;
    using PipelineLayoutType   = VulkanPipelineLayout;
};

class VulkanRenderPass : public GraphicsRenderPass<Vulkan>
{
public:
    static VulkanRenderPassHandle CreatePresentPass(const VulkanDeviceHandle& device, const VulkanSwapchainHandle& swapchain);
    VulkanRenderPass(const VulkanDeviceHandle& device);
    ~VulkanRenderPass();
    VkRenderPass GetVkRenderPass() const;
    VkFramebuffer CreateFrameBuffer(const Vector<VkImageView>& imageViews, UInt32 width, UInt32 height, UInt32 layers);
    void DestroyFrameBuffer(VkFramebuffer frameBuffer);
    
private:
    VulkanDeviceHandle               mDevice;
    Optional<VkRenderPass>           mRenderPass;
    Optional<VkRenderPassCreateInfo> mRenderPassInfo;
    Vector<VkAttachmentDescription>  mAttachments;

};

}