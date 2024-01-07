#pragma once
#include "Context/Module.h"
#include "Window.h"

namespace XHuang
{
class WindowModule : public IModule
{
public:
    WindowModule();
    ~WindowModule();
    void Initialize() override;
    void OnUpdate() override;
    WindowHandle& GetWindow() { return mWindow; }

private:
    WindowHandle mWindow;
};

} // namespace XHuang