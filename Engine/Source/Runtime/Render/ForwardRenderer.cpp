#include "ForwardRenderer.h"
#include "RenderAPI/RenderContext.h"
#include "RenderAPI/CommandBuffer.h"
#include "RenderAPI/RenderPipeline.h"

namespace XHuang
{
ForwardRenderer::ForwardRenderer(IRenderPassHandle renderPass) : IRenderer(renderPass)
{
    mRenderPass->SetUpPipelines(mPipelines);
    mRenderPass->CreateFrameBuffers();
}

void ForwardRenderer::OnUpdate()
{

}

void ForwardRenderer::OnRender(IRenderContextHandle& context)
{
    auto& commandBuffer = context->GetCommandBuffer();
    commandBuffer->CommandBegin();
    commandBuffer->CommandBeginPass(mRenderPass, context->GetRenderTarget(mRenderPass));
    for (const auto& pipeline : mPipelines) {
        commandBuffer->CommandBindPipeline(pipeline);
    }
    commandBuffer->CommandDraw(3u, 1u, 0u, 0u);
    commandBuffer->CommandEndPass();
    commandBuffer->CommandEnd();
}
} // namespace XHuang