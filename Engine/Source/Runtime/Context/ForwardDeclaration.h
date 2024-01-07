#pragma once
#include "Base/Macro.h"

namespace XHuang
{
CLASS_SHAREDPTR_DECLARE(RHI)
CLASS_SHAREDPTR_DECLARE(VulkanRHI)

CLASS_SHAREDPTR_DECLARE(IModule)
CLASS_SHAREDPTR_DECLARE(RenderModule)
CLASS_SHAREDPTR_DECLARE(IRenderer)

CLASS_SHAREDPTR_DECLARE(IRenderContext)
CLASS_SHAREDPTR_DECLARE(ICommandBuffer)
CLASS_SHAREDPTR_DECLARE(IRenderPass)
CLASS_SHAREDPTR_DECLARE(IRenderPipeline)
CLASS_SHAREDPTR_DECLARE(IRenderTarget)

CLASS_SHAREDPTR_DECLARE(WindowModule)
CLASS_SHAREDPTR_DECLARE(Window)


enum class EEngineState
{
    UnDefine = 0,
    Normal,
    Warning_ToProcess,
    Warning_ToExit,
    Error_ToExit,
    Exit,
    Max
};
} // namespace XHuang