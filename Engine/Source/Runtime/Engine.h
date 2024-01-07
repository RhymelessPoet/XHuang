#pragma once
#include "Context/EngineRuntimeContext.h"

namespace XHuang
{
class Engine
{
public:
    Engine(/* args */);
    ~Engine();
    void Initialize(EngineRuntimeContext& context);
    bool Start();
    bool Run(EngineRuntimeContext& context);
    void ShutDown();
};

}