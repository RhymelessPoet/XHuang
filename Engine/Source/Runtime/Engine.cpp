#include "Engine.h"

namespace XHuang
{
Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::Initialize(EngineRuntimeContext& context)
{
    context.InitializeModules();
}
bool Engine::Start()
{
    return false;
}
bool Engine::Run(EngineRuntimeContext& context)
{
    context.Update();
    return true;
}
void Engine::ShutDown()
{
}
}