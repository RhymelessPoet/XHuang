#include "EngineRuntimeContext.h"
#include "Module.h"

namespace XHuang
{

void EngineRuntimeContext::InitializeModules()
{
    for (auto module : mModules)
    {
        module->Initialize();
    }
}

EngineRuntimeContext& EngineRuntimeContext::AddModule(IModuleSPtr&& module)
{
    mModules.push_back(std::move(module));
    return *this;
}

} // namespace XHuang