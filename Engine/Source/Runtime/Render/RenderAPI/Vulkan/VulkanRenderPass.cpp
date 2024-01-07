#include "VulkanRenderPass.h"
#include "VulkanSwapchain.h"
#include "VulKanDevice.h"

namespace XHuang
{
VulkanRenderPassHandle VulkanRenderPass::CreatePresentPass(const VulkanDeviceHandle &device, const VulkanSwapchainHandle& swapchain)
{
    VkAttachmentDescription attachment {};
    attachment.flags          = 0;
    attachment.format         = swapchain->GetImageFormat();
    attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference attachmentRef {};
    attachmentRef.attachment = 0;
    attachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subPass {};
    subPass.flags                   = 0;
    subPass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subPass.inputAttachmentCount    = 0;
    subPass.pInputAttachments       = nullptr;
    subPass.colorAttachmentCount    = 1;
    subPass.pColorAttachments       = &attachmentRef;
    subPass.pResolveAttachments     = nullptr;
    subPass.pDepthStencilAttachment = nullptr;
    subPass.preserveAttachmentCount = 0;
    subPass.pPreserveAttachments    = nullptr;

    VkRenderPassCreateInfo renderPassInfo {};
    renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pNext           = nullptr;
    renderPassInfo.flags           = 0;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments    = &attachment;
    renderPassInfo.subpassCount    = 1;
    renderPassInfo.pSubpasses      = &subPass;
    renderPassInfo.dependencyCount = 0;
    renderPassInfo.pDependencies   = nullptr;

    VkRenderPass renderPass {};
    if (vkCreateRenderPass(device->GetVkDevice(), &renderPassInfo, nullptr, &renderPass) == VK_SUCCESS) {

    } else {
        // error log
    }
    VulkanRenderPassHandle retPass = MakeHandle<VulkanRenderPass>(device);
    retPass->mRenderPass = renderPass;

    return retPass;
}

VulkanRenderPass::VulkanRenderPass(const VulkanDeviceHandle& device) : mDevice(device)
{
    
}

VulkanRenderPass::~VulkanRenderPass()
{
    if (mRenderPass.has_value())
    {
        vkDestroyRenderPass(mDevice->GetVkDevice(), mRenderPass.value(), nullptr);
    }
}

VkRenderPass VulkanRenderPass::GetVkRenderPass() const
{
    XH_ASSERT(mRenderPass.has_value())
    return mRenderPass.value();
}
VkFramebuffer VulkanRenderPass::CreateFrameBuffer(const Vector<VkImageView> &attachments, UInt32 width, UInt32 height, UInt32 layers)
{
    XH_ASSERT(mRenderPass.has_value())
    VkFramebufferCreateInfo createInfo {};
    createInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.pNext           = nullptr;
    createInfo.flags           = 0;
    createInfo.renderPass      = mRenderPass.value();
    createInfo.attachmentCount = attachments.size();
    createInfo.pAttachments    = attachments.data();
    createInfo.width           = width;
    createInfo.height          = height;
    createInfo.layers          = layers;

    return mDevice->CreateFramebuffer(createInfo);
}
void VulkanRenderPass::DestroyFrameBuffer(VkFramebuffer frameBuffer)
{
    mDevice->DestroyFrameBuffer(frameBuffer);
}
}