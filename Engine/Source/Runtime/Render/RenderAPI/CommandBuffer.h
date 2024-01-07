#pragma once
#include "Context/ForwardDeclaration.h"
#include "Base/Base.h"
#include "Render/RenderAPI/RenderPass.h"
#include "Render/RenderAPI/RenderPipeline.h"

namespace XHuang
{
class ICommandBuffer
{
public:
    ICommandBuffer() = default;
    virtual ~ICommandBuffer() = default;

    virtual void CommandBegin() = 0;
    virtual void CommandBeginPass(const IRenderPassHandle& passHandle, const IRenderTargetHandle& renderTargetHandle) = 0;
    virtual void CommandBindPipeline(const IRenderPipelineHandle& pipelineHandle) = 0;
    virtual void CommandDraw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex, UInt32 firstInstance) = 0;
    virtual void CommandEndPass() = 0; 
    virtual void CommandEnd()   = 0;

private:

};

template <typename CommandBufferType>
class CommandBuffer final : public ICommandBuffer
{
public:
    using RenderPassType       = typename CommandBufferType::RenderPassType;
    using GraphicsPipelineType = typename CommandBufferType::GraphicsPipelineType;
    using FrameBufferType      = typename CommandBufferType::FrameBufferType;
    using CommandBufferHandleType = Handle<CommandBufferType>;
    CommandBuffer(CommandBufferHandleType commandBuffer) : mCommandBuffer(std::move(commandBuffer)) {}
    ~CommandBuffer() = default;
    void CommandBegin() override { mCommandBuffer->CommandBegin(); }
    void CommandBeginPass(const IRenderPassHandle& passHandle, const IRenderTargetHandle& renderTargetHandle) override
    {
        auto renderPassHandle = std::static_pointer_cast<RenderPass<RenderPassType>>(passHandle);
        auto _RenderTargetHandle = std::static_pointer_cast<RenderTarget<FrameBufferType>>(renderTargetHandle);
        mCommandBuffer->CommandBeginPass(renderPassHandle->GetRHIRenderPass(), _RenderTargetHandle->GetRHIFrameBuffer());
    }

    void CommandBindPipeline(const IRenderPipelineHandle& pipelineHandle)
    {
        auto renderPipelineHandle = std::static_pointer_cast<RenderPipeline<GraphicsPipelineType>>(pipelineHandle);
        mCommandBuffer->CommandBindPipeline(renderPipelineHandle->GetRHIRenderPipeline());
    }

    void CommandDraw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex, UInt32 firstInstance) override
    {
        mCommandBuffer->CommandDraw(vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void CommandEndPass() override
    {
        mCommandBuffer->CommandEndPass();
    }

    void CommandEnd() override { mCommandBuffer->CommandEnd(); }

private:
    CommandBufferHandleType mCommandBuffer;
};

} // namespace XHuang