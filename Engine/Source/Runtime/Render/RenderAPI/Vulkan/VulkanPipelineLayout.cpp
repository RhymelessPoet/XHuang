#include "VulkanPipelineLayout.h"
#include "VulKanDevice.h"

namespace XHuang
{

VulkanPipelineLayout::VulkanPipelineLayout(const VulkanDeviceHandle& device) : mDevice(device)
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext                  = nullptr;
    pipelineLayoutInfo.flags                  = 0;
    pipelineLayoutInfo.setLayoutCount         = 0; // Optional
    pipelineLayoutInfo.pSetLayouts            = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges    = 0; // Optional
    VkPipelineLayout pipelineLayout {};
    if (vkCreatePipelineLayout(mDevice->GetVkDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) == VK_SUCCESS) {
        mLayout = pipelineLayout;
    } else {
        // log error
    }
}

VulkanPipelineLayout::~VulkanPipelineLayout()
{
    if (mLayout.has_value()) {
        vkDestroyPipelineLayout(mDevice->GetVkDevice(), mLayout.value(), nullptr);
    }
}

}