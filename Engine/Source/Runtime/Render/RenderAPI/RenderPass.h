#pragma once
#include "Base/Base.h"
#include "RHI.h"
#include "RenderAPI_Definition.h"
#include "Vulkan/VulkanShader.h"
#include "FileSystem/FileSystem.h"

namespace XHuang
{

class IRenderPass
{
public:
    IRenderPass() = default;
    virtual ~IRenderPass() = default;
    virtual void SetUpPipelines(Vector<IRenderPipelineHandle>& pipelineHandles) = 0;
    virtual void CreateFrameBuffers() = 0;
};

template <typename RenderPassType>
class RenderPass : public IRenderPass
{
public:
    using RenderPassHandleType = Handle<RenderPassType>;
    using GraphicsPipelineHandle = Handle<typename RenderPassType::GraphicsPipelineType>;
    using FrameBufferHandleType = Handle<typename RenderPassType::FrameBufferType>;
    using ImageViewHandleType = Handle<typename RenderPassType::ImageViewType>;

    RenderPass(RHIHandle rhi) : mRHI(std::move(rhi))
    {
        auto _rhi = std::static_pointer_cast<RenderPassType::RHIType>(mRHI);
        mRenderPass = RenderPassType::CreatePresentPass(_rhi->mDevice, _rhi->mSwapchain);
        mViewportExtent = _rhi->mSwapchain->GetExtent();
    }
    ~RenderPass() = default;
    void SetUpPipelines(Vector<IRenderPipelineHandle>& pipelineHandles) override
    {
        static FilePath vertexShaderPath("Engine/Shader/Generated/hello_triangle.vert.spv");
        static FilePath fragmentShaderPath("Engine/Shader/Generated/hello_triangle.frag.spv");
        auto _rhi = std::static_pointer_cast<RenderPassType::RHIType>(mRHI);
        mPipelines.resize(1);
        Vector<char> vertexShaderCode;
        Vector<char> fragmentShaderCode;
        FileSystem::Load_SPV_File(vertexShaderPath, vertexShaderCode);
        FileSystem::Load_SPV_File(fragmentShaderPath, fragmentShaderCode);
        typename RenderPassType::ShaderType vertexShader(EVulkanShaderType::Vertex, _rhi->mDevice, vertexShaderCode);
        typename RenderPassType::ShaderType fragmentShader(EVulkanShaderType::Fragment, _rhi->mDevice, fragmentShaderCode);
        Vector2 viewportSize = _rhi->mSwapchain->GetExtent();
        mPipelines[0] = MakeHandle<RenderPassType::GraphicsPipelineType>(_rhi->mDevice);
        mPipelines[0]
        ->StartBuild(VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT)
        .AddStatge(vertexShader)
        .AddStatge(fragmentShader)
        .SetUpVertextInputState()
        .SetUpInputAssemblyState()
        .SetUpViewportState(mViewportExtent)
        .SetUpRasterizationState()
        .SetUpMultisampleState()
        .SetUpDepthStencilState()
        .SetUpLayout(RenderPassType::PipelineLayoutType(_rhi->mDevice))
        .SetUpColorBlendState()
        .SetUpRenderPass(mRenderPass, 0)
        .EndBuild();

        pipelineHandles.push_back(MakeHandle<RenderPipeline<RenderPassType::GraphicsPipelineType>>(mPipelines[0]));
    }

    void CreateFrameBuffers() override
    {
        auto _rhi = std::static_pointer_cast<RenderPassType::RHIType>(mRHI);
        for (const auto& imageView : _rhi->mSwapchain->GetImageViews()) {
            Vector<ImageViewHandleType> attachments = { imageView };
            mFrameBuffers.emplace_back(MakeHandle<RenderPassType::FrameBufferType>(mRenderPass, std::move(attachments), mViewportExtent, 1));
        }
    }

    RenderPassHandleType& GetRHIRenderPass() { return mRenderPass; }
    FrameBufferHandleType& GetFrameBuffer(UInt32 index)
    { 
        XH_ASSERT(index < mFrameBuffers.size())
        return mFrameBuffers[index];
    }

private:
    RHIHandle                      mRHI;
    RenderPassHandleType           mRenderPass;
    Vector<GraphicsPipelineHandle> mPipelines;
    Vector<FrameBufferHandleType>  mFrameBuffers;
    Extent2D                       mViewportExtent;
};

}