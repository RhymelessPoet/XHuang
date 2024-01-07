#pragma once
#include <functional>
#include "ForwardDeclaration.h"

namespace XHuang
{
class IModule
{
public:
    virtual void Initialize() = 0;
    virtual void OnUpdate() = 0;
    void SetContextChangeCallback(std::function<void(EEngineState state)> func) { _ChangeContextState = func; }
    
protected:
    std::function<void(EEngineState state)> _ChangeContextState;
};

}