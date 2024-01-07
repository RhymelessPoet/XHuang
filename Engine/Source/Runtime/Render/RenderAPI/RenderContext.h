#pragma once
#include "Base/Base.h"
#include "Context/ForwardDeclaration.h"
#include "CommandBuffer.h"
#include "RenderTarget.h"
#include "RenderPass.h"

namespace XHuang
{
    
class IRenderContext
{
public:
    IRenderContext() = default;
    virtual ~IRenderContext() = default;
    virtual ICommandBufferHandle& GetCommandBuffer() = 0;
    virtual IRenderTargetHandle GetRenderTarget(const IRenderPassHandle& renderPassHandle) const = 0;
    virtual void Synchronous() = 0;
    virtual void Submit() = 0;
    virtual void Swap() = 0;
};

template <typename GraphicsContextType>
class RenderContext : public IRenderContext
{
public:
    using CommandBufferType = typename GraphicsContextType::CommandBufferType;
    using RenderPassType = typename GraphicsContextType::RenderPassType;
    using FrameBufferType = typename GraphicsContextType::FrameBufferType;
    using ContextHandleType = Handle<GraphicsContextType>;

    RenderContext(ContextHandleType context) : mGraphicsContext(std::move(context))
    {
        mCommandBuffer = MakeHandle<CommandBuffer<CommandBufferType>>(mGraphicsContext->GetCommandBuffer());
    }

    ICommandBufferHandle& GetCommandBuffer() { return  mCommandBuffer; }

    IRenderTargetHandle GetRenderTarget(const IRenderPassHandle& renderPassHandle) const override
    {
        UInt32 nextFrameBufferIndex = mGraphicsContext->GetNextFrameBufferIndex();
        auto rhiRenderPassHandle = std::static_pointer_cast<RenderPass<RenderPassType>>(renderPassHandle);
        auto rhiFrameBufferHandle = rhiRenderPassHandle->GetFrameBuffer(nextFrameBufferIndex);
        
        return MakeHandle<RenderTarget<FrameBufferType>>(rhiFrameBufferHandle);
    }

    void Synchronous() override
    {
        mGraphicsContext->Synchronous();
    }

    void Submit() override
    {
        mGraphicsContext->Submit();
    }

    void Swap() override
    {
        mGraphicsContext->Swap();
    }

private:
    ICommandBufferHandle mCommandBuffer;
    ContextHandleType mGraphicsContext;
};

} // namespace XHuang
