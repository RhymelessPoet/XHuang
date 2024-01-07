#pragma once
#include "Base/Base.h"
#include "VulkanForwardDeclaration.h"
#include "Render/RenderAPI/RenderAPI_Definition.h"

namespace XHuang
{

class VulkanGraphicsPipeline
{
public:
    VulkanGraphicsPipeline(const VulkanDeviceHandle& device);
    ~VulkanGraphicsPipeline();
    VulkanGraphicsPipeline& StartBuild(VkPipelineCreateFlags flags);
    VulkanGraphicsPipeline& AddStatge(const VulkanShader& shader);
    VulkanGraphicsPipeline& SetUpVertextInputState();
    VulkanGraphicsPipeline& SetUpInputAssemblyState();
    VulkanGraphicsPipeline& SetUpTessellationState();
    VulkanGraphicsPipeline& SetUpViewportState(Extent2D viewportSize);
    VulkanGraphicsPipeline& SetUpRasterizationState();
    VulkanGraphicsPipeline& SetUpMultisampleState();
    VulkanGraphicsPipeline& SetUpDepthStencilState();
    VulkanGraphicsPipeline& SetUpColorBlendState();
    VulkanGraphicsPipeline& SetUpDynamicState();
    VulkanGraphicsPipeline& SetUpLayout(const VulkanPipelineLayout& layout);
    VulkanGraphicsPipeline& SetUpRenderPass(const VulkanRenderPassHandle& renderPass, UInt32 subpassIndex);
    VulkanGraphicsPipeline& SetUpBasePipeline(const VulkanGraphicsPipelineHandle& basePipeline);
    VulkanGraphicsPipeline& SetUpPipelineCache();
    void EndBuild();
    VkPipeline GetVkPipeline() const { return mPipeline.value(); }

private:
    template <typename T>
    inline T* StateInfo(Optional<T>& state) { return state.has_value() ? &(state.value()) : nullptr; }

private:
    struct FixedStages
    {
        Optional<VkPipelineVertexInputStateCreateInfo>   VertexInput;     // 顶点输入
        Optional<VkPipelineInputAssemblyStateCreateInfo> InputAssembly;   // 输入组装
        Optional<VkPipelineTessellationStateCreateInfo>  Tessellation;    // 细分
        Optional<VkPipelineViewportStateCreateInfo>      Viewport;        // 视口
        Optional<VkPipelineRasterizationStateCreateInfo> Rasterization;   // 光栅化
        Optional<VkPipelineMultisampleStateCreateInfo>   Multisample;     // 多重采样
        Optional<VkPipelineDepthStencilStateCreateInfo>  DepthStencil;    // 深度与模板
        Optional<VkPipelineColorBlendStateCreateInfo>    ColorBlend;      // 颜色与混合
        Optional<VkPipelineDynamicStateCreateInfo>       Dynamic;         // 动态
    };
    
    VulkanDeviceHandle                      mDevice;
    Optional<VkGraphicsPipelineCreateInfo>  mVkCreateInfo;
    FixedStages                             mFixedStages;
    Vector<VkPipelineShaderStageCreateInfo> mStages;
    Vector<VkDynamicState>                  mDynamicStates;
    Optional<VkPipelineCache>               mCache;
    Optional<VkPipeline>                    mPipeline;
    VulkanGraphicsPipelineHandle            mBasePipeline = nullptr;
    VulkanRenderPassHandle                  mRenderPass = nullptr;
    UInt32                                  mSubpassIndex = 0;
};

}