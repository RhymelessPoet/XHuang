#pragma once
#include "Base/Base.h"
#include "ForwardDeclaration.h"
#include <typeinfo>

namespace XHuang
{

class EngineRuntimeContext
{
public:
    EngineRuntimeContext() = default;
    ~EngineRuntimeContext() = default;
    void InitializeModules();
    EngineRuntimeContext& AddModule(IModuleHandle module);
    void Update();
    /*template <typename ModuleType>
    Handle<ModuleType> Get() const
    {
        for ()
    }*/
    EEngineState GetEngineState() const { return mEngineState; }
    void SetEngineState(EEngineState state) { mEngineState = state; }

private:
    EEngineState mEngineState = EEngineState::UnDefine;
    Vector<IModuleHandle> mModules;
};
}
