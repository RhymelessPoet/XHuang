#include "RenderModule.h"
#include "RHI/VulkanRHI.h"
namespace XHuang
{
RenderModule::~RenderModule()
{

}

void RenderModule::Initialize()
{
    mRHI = std::make_shared<VulkanRHI>();
    RHI_InitInfo RHI_InitInfo = { 0, "XHuang_Render", 0, "XHuang" };
    mRHI->Initialize(RHI_InitInfo);
}
}

