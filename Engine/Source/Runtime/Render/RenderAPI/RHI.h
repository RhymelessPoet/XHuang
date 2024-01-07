#pragma once

#include "Base/Base.h"
#include "Base/String.h"
#include "Window/Window.h"

namespace XHuang
{

struct RHI_InitInfo
{
    UInt32  ApplicationVersion;
    String  ApplicationName;
    UInt32  EngineVersion;
    String  EngineName = "XHuang";
    WindowHandle Window = nullptr;
    
};

class RHI
{
public:
    RHI() = default;
    virtual ~RHI() = 0;

    virtual void Initialize(const RHI_InitInfo& rhiInfo) = 0;
    virtual void Reset() = 0;
    virtual IRenderContextHandle MakeContext() = 0;

public:
    UInt32 mRHI_API_Version = 0;
};
inline RHI::~RHI() = default;
} // namespace XHuang