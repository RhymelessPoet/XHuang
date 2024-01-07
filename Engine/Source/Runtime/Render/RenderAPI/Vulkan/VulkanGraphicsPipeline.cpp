#include "VulkanGraphicsPipeline.h"
#include "VulkanShader.h"
#include "VulkanPipelineLayout.h"
#include "VulkanRenderPass.h"
#include "VulKanDevice.h"

namespace XHuang
{
VulkanGraphicsPipeline::VulkanGraphicsPipeline(const VulkanDeviceHandle& device) : mDevice(device)
{
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
{
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::StartBuild(VkPipelineCreateFlags flags)
{
    VkGraphicsPipelineCreateInfo pipeInfo {};
    pipeInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeInfo.pNext = nullptr;
    pipeInfo.flags = flags;
    mVkCreateInfo = pipeInfo;
    return *this;
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::AddStatge(const VulkanShader &shader)
{
    VkPipelineShaderStageCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.stage = static_cast<VkShaderStageFlagBits>(shader.GetShaderType());
    createInfo.module = shader.GetShaderModule();
    createInfo.pName = "main";
    VkSpecializationInfo specializationInfo {};
    createInfo.pSpecializationInfo = shader.GetSpecializationInfo(specializationInfo) ? &specializationInfo : nullptr;
    mStages.push_back(createInfo);
    return *this;
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetUpVertextInputState()
{
    XH_ASSERT(!mFixedStages.VertexInput.has_value())
    VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.pNext = nullptr;
    vertexInputInfo.flags = 0;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    mFixedStages.VertexInput = vertexInputInfo;
    return *this;
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetUpInputAssemblyState()
{
    XH_ASSERT(!mFixedStages.InputAssembly.has_value())
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo {};
    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.pNext = nullptr;
    inputAssemblyInfo.flags = 0;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyInfo.primitiveRestartEnable = false;

    mFixedStages.InputAssembly = inputAssemblyInfo;
    return *this;
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetUpTessellationState()
{
    XH_ASSERT(!mFixedStages.Tessellation.has_value())
    VkPipelineTessellationStateCreateInfo tessellationInfo {};
    tessellationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    tessellationInfo.pNext = nullptr;
    tessellationInfo.flags = 0;
    tessellationInfo.patchControlPoints = 0;

    mFixedStages.Tessellation = tessellationInfo;
    return *this;
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetUpViewportState(Extent2D viewportSize)
{
    XH_ASSERT(!mFixedStages.Viewport.has_value())
    static VkViewport viewport {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(viewportSize.X);
    viewport.height = static_cast<float>(viewportSize.Y);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    static VkRect2D scissor {};
    scissor.offset = { 0u, 0u };
    scissor.extent = { static_cast<UInt32>(viewportSize.X), static_cast<UInt32>(viewportSize.Y) };

    VkPipelineViewportStateCreateInfo viewportInfo {};
    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.pNext = nullptr;
    viewportInfo.flags = 0;
    viewportInfo.viewportCount = 1;
    viewportInfo.pViewports = &viewport;
    viewportInfo.scissorCount = 1;
    viewportInfo.pScissors = &scissor;

    mFixedStages.Viewport = viewportInfo;
    return *this;
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetUpRasterizationState()
{
    XH_ASSERT(!mFixedStages.Rasterization.has_value())
    VkPipelineRasterizationStateCreateInfo rasterizationInfo {};
    rasterizationInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationInfo.pNext                   = nullptr;
    rasterizationInfo.flags                   = 0;
    rasterizationInfo.depthClampEnable        = VK_FALSE;
    rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizationInfo.polygonMode             = VK_POLYGON_MODE_FILL;
    rasterizationInfo.cullMode                = 0;
    rasterizationInfo.frontFace               = VK_FRONT_FACE_CLOCKWISE;
    rasterizationInfo.depthBiasEnable         = false;
    rasterizationInfo.depthBiasConstantFactor = 0.0f;
    rasterizationInfo.depthBiasClamp          = 0.0f;
    rasterizationInfo.depthBiasSlopeFactor    = 0.0f;
    rasterizationInfo.lineWidth               = 1.0f;

    mFixedStages.Rasterization = rasterizationInfo;
    return *this;
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetUpMultisampleState()
{
    VkPipelineMultisampleStateCreateInfo multisampleInfo {};
    multisampleInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleInfo.pNext                 = nullptr;
    multisampleInfo.flags                 = 0;
    multisampleInfo.sampleShadingEnable   = VK_FALSE;
    multisampleInfo.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
    multisampleInfo.minSampleShading      = 1.0f;                  // Optional
    multisampleInfo.pSampleMask           = nullptr;               // Optional
    multisampleInfo.alphaToCoverageEnable = VK_FALSE;              // Optional
    multisampleInfo.alphaToOneEnable      = VK_FALSE;              // Optional

    mFixedStages.Multisample = multisampleInfo;
    return *this;
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetUpDepthStencilState()
{
    VkStencilOpState frontStencilOp {};
    frontStencilOp.failOp      = VK_STENCIL_OP_KEEP;
    frontStencilOp.passOp      = VK_STENCIL_OP_KEEP;
    frontStencilOp.depthFailOp = VK_STENCIL_OP_KEEP;
    frontStencilOp.compareOp   = VK_COMPARE_OP_ALWAYS;
    frontStencilOp.compareMask = 0xFF;
    frontStencilOp.writeMask   = 0xFF;
    frontStencilOp.reference   = 1;
    
    VkStencilOpState backStencilOp = frontStencilOp;

    VkPipelineDepthStencilStateCreateInfo depthStencilInfo {};
    depthStencilInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilInfo.pNext                 = nullptr;
    depthStencilInfo.flags                 = 0;
    depthStencilInfo.depthTestEnable       = VK_FALSE;
    depthStencilInfo.depthWriteEnable      = VK_FALSE;
    depthStencilInfo.depthCompareOp        = VK_COMPARE_OP_NEVER;
    depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilInfo.stencilTestEnable     = VK_FALSE;
    depthStencilInfo.front                 = frontStencilOp;
    depthStencilInfo.back                  = backStencilOp;
    depthStencilInfo.minDepthBounds        = 0.0f;
    depthStencilInfo.maxDepthBounds        = 0.0f;

    mFixedStages.DepthStencil = depthStencilInfo;
    return *this;
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetUpColorBlendState()
{
    static VkPipelineColorBlendAttachmentState colorBlendAttachment {};
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    colorBlendAttachment.colorBlendOp        = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    colorBlendAttachment.alphaBlendOp        = VK_BLEND_OP_ADD;
    colorBlendAttachment.colorWriteMask      = 0xF;

    const float constantColor[4] =  { 0.0f, 0.0f, 0.0f, 0.0f };

    VkPipelineColorBlendStateCreateInfo colorBlendInfo {};
    colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendInfo.pNext = nullptr;
    colorBlendInfo.flags = 0;
    colorBlendInfo.logicOpEnable = VK_FALSE;
    colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
    colorBlendInfo.attachmentCount = 1;
    colorBlendInfo.pAttachments = &colorBlendAttachment;
    colorBlendInfo.blendConstants[0] = constantColor[0];
    colorBlendInfo.blendConstants[1] = constantColor[1];
    colorBlendInfo.blendConstants[2] = constantColor[2];
    colorBlendInfo.blendConstants[3] = constantColor[3];

    mFixedStages.ColorBlend = colorBlendInfo;
    return *this;
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetUpDynamicState()
{
    VkPipelineDynamicStateCreateInfo dynamicInfo {};
    dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicInfo.pNext = nullptr;
    dynamicInfo.flags = 0;
    dynamicInfo.dynamicStateCount = static_cast<UInt32>(mDynamicStates.size());
    dynamicInfo.pDynamicStates = mDynamicStates.data();

    mFixedStages.Dynamic = dynamicInfo;
    return *this;
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetUpLayout(const VulkanPipelineLayout& layout)
{
    mVkCreateInfo.value().layout = layout.GetVkLayout();
    return *this;
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetUpRenderPass(const VulkanRenderPassHandle& renderPass, UInt32 subpassIndex)
{
    mRenderPass = renderPass;
    mSubpassIndex = subpassIndex;
    return *this;
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetUpBasePipeline(const VulkanGraphicsPipelineHandle& basePipeline)
{
    mBasePipeline = basePipeline;
    return *this;
}

VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetUpPipelineCache()
{
    VkPipelineCacheCreateInfo createInfo {};
    createInfo.sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    createInfo.pNext           = nullptr;
    createInfo.flags           = 0;
    createInfo.initialDataSize = 0;
    createInfo.pInitialData    = nullptr;

    VkPipelineCache pipelineCache {};
    if (vkCreatePipelineCache(mDevice->GetVkDevice(), &createInfo, nullptr, &pipelineCache) == VK_SUCCESS) {
        mCache = pipelineCache;
    } else {
        // log error
    }
    return *this;
}

void VulkanGraphicsPipeline::EndBuild()
{
    XH_ASSERT(mVkCreateInfo.has_value())
    VkGraphicsPipelineCreateInfo& pipelineInfo = mVkCreateInfo.value();
    pipelineInfo.stageCount          = static_cast<UInt32>(mStages.size());
    pipelineInfo.pStages             = mStages.data();
    pipelineInfo.pVertexInputState   = StateInfo(mFixedStages.VertexInput);
    pipelineInfo.pInputAssemblyState = StateInfo(mFixedStages.InputAssembly);
    pipelineInfo.pTessellationState  = StateInfo(mFixedStages.Tessellation);
    pipelineInfo.pViewportState      = StateInfo(mFixedStages.Viewport);
    pipelineInfo.pRasterizationState = StateInfo(mFixedStages.Rasterization);
    pipelineInfo.pMultisampleState   = StateInfo(mFixedStages.Multisample);
    pipelineInfo.pDepthStencilState  = StateInfo(mFixedStages.DepthStencil);
    pipelineInfo.pColorBlendState    = StateInfo(mFixedStages.ColorBlend);
    pipelineInfo.pDynamicState       = StateInfo(mFixedStages.Dynamic);
    pipelineInfo.renderPass          = mRenderPass->GetVkRenderPass();
    pipelineInfo.subpass             = mSubpassIndex;
    pipelineInfo.basePipelineHandle  = mBasePipeline != nullptr && mBasePipeline->mPipeline.has_value() ? mBasePipeline->mPipeline.value() : VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex   = -1;
    
    VkPipeline pipeline {};
    if (vkCreateGraphicsPipelines(mDevice->GetVkDevice(), mCache.has_value() ? mCache.value() : VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) == VK_SUCCESS) {
        mPipeline = pipeline;
    } else {
        // error log
    }
}

} // namespace XHuang