#pragma once
#include "Base/Base.h"
#include "Base/String.h"

namespace XHuang
{
struct RHI_InitInfo
{
    UInt32 mApplicationVersion;
    String mApplicationName;
    UInt32 mEngineVersion;
    String mEngineName = "XHuang";
};

class RHI
{
public:
    RHI() = default;
    virtual ~RHI() = 0;

    virtual void Initialize(const RHI_InitInfo& rhiInfo) = 0;

public:
    UInt32 mRHI_API_Version;
};
inline RHI::~RHI() = default;
} // namespace XHuang