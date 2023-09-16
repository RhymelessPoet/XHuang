#pragma once
#include "Base/Base.h"
#include "ForwardDeclaration.h"

namespace XHuang
{
class EngineRuntimeContext
{
public:
    EngineRuntimeContext() = default;
    ~EngineRuntimeContext() = default;
    void InitializeModules();
    EngineRuntimeContext& AddModule(ModuleSPtr&& module);

private:
    Vector<ModuleSPtr> mModules;
};
}
