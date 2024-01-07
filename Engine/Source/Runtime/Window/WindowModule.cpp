#include "WindowModule.h"

namespace XHuang
{
WindowModule::WindowModule()
{
	mWindow = MakeHandle<Window>();
	mWindow->Initialize(WindowCreateInfo{});
}

WindowModule::~WindowModule()
{
}

void WindowModule::Initialize()
{
}

void WindowModule::OnUpdate()
{
	if (mWindow->ShouldExit()) {
		_ChangeContextState(EEngineState::Exit);
	}
}

} // namespace XHuang