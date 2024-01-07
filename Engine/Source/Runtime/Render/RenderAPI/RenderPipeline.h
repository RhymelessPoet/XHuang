#pragma once
#include "Base/Base.h"
#include "Context/ForwardDeclaration.h"

namespace XHuang
{

class IRenderPipeline
{
public:
    IRenderPipeline() = default;
};

template <typename RenderPipelineType>
class RenderPipeline : public IRenderPipeline
{
public:
    using RenderPipelineHandleType = Handle<RenderPipelineType>;
    RenderPipeline(RenderPipelineHandleType renderPipeline) : mRenderPipeline(std::move(renderPipeline)) {}
    
    RenderPipelineHandleType& GetRHIRenderPipeline() { return mRenderPipeline; }

private:
    RenderPipelineHandleType mRenderPipeline;
};


} // namespace XHuang
