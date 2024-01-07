#include "RenderModule.h"
#include "RenderAPI/Vulkan/VulkanRHI.h"
#include "RenderAPI/RenderContext.h"
#include "RenderAPI/RenderPass.h"
#include "ForwardRenderer.h"
#include "RenderAPI/Vulkan/VulkanRenderPass.h"
#include "RenderAPI/Vulkan/VulkanGraphicsPipeline.h"
#include "RenderAPI/Vulkan/VulkanFrameBuffer.h"
#include "RenderAPI/Vulkan/VulkanPipelineLayout.h"

namespace XHuang
{
RenderModule::~RenderModule()
{

}

void RenderModule::Initialize()
{
    mRHI = std::make_shared<VulkanRHI>();
    RHI_InitInfo RHI_InitInfo = { 0, "XHuang_Render", 0, "XHuang", mWindow };
    mRHI->Initialize(RHI_InitInfo);
    auto renderPassHandle = MakeHandle<RenderPass<VulkanRenderPass>>(mRHI);
    mRenderers.emplace_back(MakeHandle<ForwardRenderer>(renderPassHandle));
    mRenderContext = mRHI->MakeContext();
}

void RenderModule::OnUpdate()
{
    for (auto& render : mRenderers) {
        render->OnUpdate();
    }
    mRenderContext->Synchronous();
    for (auto& render : mRenderers) {
        render->OnRender(mRenderContext);
    }
    mRenderContext->Submit();
    mRenderContext->Swap();
}

}

