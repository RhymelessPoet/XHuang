#pragma once
#include "VulkanForwardDeclaration.h"
#include "Base/Base.h"

namespace XHuang
{

class VulkanFence
{
public:
    VulkanFence(VulkanDeviceHandle device, bool isSignaled = false);
    ~VulkanFence();
    VkFence GetVkFence() const { XH_ASSERT(mFence.has_value()) return mFence.value(); }
    bool IsSignaled() const { return mIsSignaled; }
    void Wait();
    void Reset();

private:
    bool mIsSignaled = false;
    VulkanDeviceHandle mDevice = nullptr;
    Optional<VkFence> mFence;
};

} // namespace XHuang
