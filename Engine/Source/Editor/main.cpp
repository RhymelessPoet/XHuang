#include <iostream>
#include "Runtime/Engine.h"
#include "Context/EngineRuntimeContext.h"
#include "Render/RenderModule.h"
int main()
{
    XHuang::EngineRuntimeContext context;
    XHuang::Engine engine;
    XHuang::ModuleSPtr module = std::make_shared<XHuang::RenderModule>();
    context.AddModule(std::move(module));
    engine.Initialize(context);
    system("pause");
    return 0;
}