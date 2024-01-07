#pragma once
#include "Renderer.h"
#include "Context/ForwardDeclaration.h"

namespace XHuang
{

class ForwardRenderer : public IRenderer
{
public:
    ForwardRenderer(IRenderPassHandle renderPass);
    ~ForwardRenderer() = default;
    void OnUpdate() override;
    void OnRender(IRenderContextHandle& context) override;

};

} // namespace XHuang