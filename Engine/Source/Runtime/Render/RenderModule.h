#pragma once
#include "Context/Module.h"
#include "Context/ForwardDeclaration.h"

namespace XHuang
{

class RenderModule : public Module
{
public:
    RenderModule(/* args */) = default;
    virtual ~RenderModule();
    void Initialize() override;

private:
    RHISPtr mRHI;
};

} // namespace XHuang