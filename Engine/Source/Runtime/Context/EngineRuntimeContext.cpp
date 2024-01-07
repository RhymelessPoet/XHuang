#include "EngineRuntimeContext.h"
#include "Module.h"

namespace XHuang
{

void EngineRuntimeContext::InitializeModules()
{
    for (auto& module : mModules)
    {
        module->Initialize();
    }
    mEngineState = EEngineState::Normal;
}

EngineRuntimeContext& EngineRuntimeContext::AddModule(IModuleHandle module)
{
    module->SetContextChangeCallback(std::bind(&EngineRuntimeContext::SetEngineState, this, std::placeholders::_1));
    mModules.push_back(std::move(module));
    return *this;
}

void EngineRuntimeContext::Update()
{
    for (auto& module : mModules) {
        module->OnUpdate();
    }
}

} // namespace XHuang