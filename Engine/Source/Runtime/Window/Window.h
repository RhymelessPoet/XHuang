#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Base/Base.h"
#include "Base/String.h"
#include "Context/ForwardDeclaration.h"

namespace XHuang
{
struct WindowCreateInfo
{
    UInt32 Width        = 1280u;
    UInt32 Height       = 720u;
    String Title        = "XuanHuang";
    bool   IsFullScreen = false;
};

class Window
{
public:
    Window() = default;
    ~Window();
    void Initialize(const WindowCreateInfo& windowInfo);
    bool ShouldExit();
    GLFWwindow* GetGLFWwindow() const { return mWindow; }
    UInt32 GetWindowWidth() const { return mWidth; }
    UInt32 GetWindowHeight() const { return mHeight; }
    void Reset();

private:
    UInt32 mWidth        = 0u;
    UInt32 mHeight       = 0u;
    String mTitle        = "XuanHuang";
    bool   mIsFullScreen = false;
    GLFWwindow* mWindow  = nullptr;

};

} // namespace XHuang