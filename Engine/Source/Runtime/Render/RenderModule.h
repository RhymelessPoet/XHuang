#pragma once
#include "Base/Containers.h"
#include "Context/Module.h"
#include "Context/ForwardDeclaration.h"

namespace XHuang
{

class RenderModule : public IModule
{
public:
    RenderModule() = default;
    virtual ~RenderModule();
    void Initialize() override;
    void OnUpdate() override;
    void SetWindow(WindowHandle window) { mWindow = std::move(window); }

private:
    WindowHandle mWindow;
    RHIHandle mRHI;
    IRenderContextHandle mRenderContext;
    Vector<IRendererHandle> mRenderers;
};

} // namespace XHuang