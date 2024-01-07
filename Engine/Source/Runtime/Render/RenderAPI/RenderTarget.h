#pragma once
#include "Base/Base.h"

namespace XHuang
{
    
class IRenderTarget
{
public:
    IRenderTarget() = default;
};

template <typename FrameBufferType>
class RenderTarget : public IRenderTarget
{
public:
    using FrameBufferHandleType = Handle<FrameBufferType>;
    RenderTarget(FrameBufferHandleType frameBufferHandle) : mFrameBuffer(std::move(frameBufferHandle)) {}

    const FrameBufferHandleType& GetRHIFrameBuffer() const { return mFrameBuffer; }

private:
    FrameBufferHandleType mFrameBuffer;
};



} // namespace XHuang
