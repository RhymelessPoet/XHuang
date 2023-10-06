#include "Window.h"

namespace XHuang
{
XHuang::Window::Window()
{
    
}
Window::~Window()
{
    if (mWindow != nullptr) {
        glfwDestroyWindow(mWindow);
        glfwTerminate(); 
    }
}

void Window::Initialize(const WindowCreateInfo &windowInfo)
{
    if (glfwInit() != GLFW_TRUE) {
        // log
        return;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    mWindow = glfwCreateWindow(windowInfo.Width, windowInfo.Height, windowInfo.Title.c_str(), nullptr, nullptr);
    if (mWindow == nullptr) {
        // log
        glfwTerminate();
        return;
    }
    mWidth = windowInfo.Width;
    mHeight = windowInfo.Height;
    mTitle = windowInfo.Title;
    mIsFullScreen = windowInfo.IsFullScreen;
}

void Window::Reset()
{
     if (mWindow != nullptr) {
        glfwDestroyWindow(mWindow);
        glfwTerminate(); 
    }
}

} // namespace XHuang