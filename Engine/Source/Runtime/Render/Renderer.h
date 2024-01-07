#pragma once
#include "Base/Containers.h"
#include "Context/ForwardDeclaration.h"

namespace XHuang
{

class IRenderer
{
public:
    IRenderer(IRenderPassHandle renderPass) : mRenderPass(std::move(renderPass)) {}
    virtual ~IRenderer() = default;

    virtual void OnUpdate() = 0;
    virtual void OnRender(IRenderContextHandle& context) = 0;

protected:
    IRenderPassHandle mRenderPass;
    Vector<IRenderPipelineHandle> mPipelines;
};

} // namespace XHuang