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
void Engine::ShutDown()
{
}
}