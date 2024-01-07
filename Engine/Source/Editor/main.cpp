#include <iostream>
#include "Runtime/Engine.h"
#include "Context/EngineRuntimeContext.h"
#include "Render/RenderModule.h"
#include "Window/WindowModule.h"

int main()
{
    XHuang::EngineRuntimeContext context;
    XHuang::Engine engine;
    XHuang::RenderModuleHandle renderModule = XHuang::MakeHandle<XHuang::RenderModule>();
    XHuang::WindowModuleHandle windowModule = XHuang::MakeHandle<XHuang::WindowModule>();
    renderModule->SetWindow(windowModule->GetWindow());
    context.AddModule(std::move(windowModule)).AddModule(std::move(renderModule));

    engine.Initialize(context);
    engine.Start();
    while (context.GetEngineState() == XHuang::EEngineState::Normal) {
        engine.Run(context);
    }
    engine.ShutDown();
    system("pause");
    return 0;
}