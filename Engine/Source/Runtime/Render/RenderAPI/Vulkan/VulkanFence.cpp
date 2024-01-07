#include "VulkanFence.h"
#include "VulKanDevice.h"

namespace XHuang
{
VulkanFence::VulkanFence(VulkanDeviceHandle device, bool isSignaled) : mDevice(std::move(device)), mIsSignaled(isSignaled)
{
	VkFenceCreateInfo createInfo {};
	createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = !isSignaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

	VkFence fence = mDevice->CreateFence(createInfo);
	if (fence != VK_NULL_HANDLE) {
		mFence = fence;
	}
	else {
		// log error
	}
}

VulkanFence::~VulkanFence()
{
	if (mFence.has_value()) {
		mDevice->DestroyFence(mFence.value());
	}
}

void VulkanFence::Wait()
{
	XH_ASSERT(mFence.has_value())
	vkWaitForFences(mDevice->GetVkDevice(), 1, &mFence.value(), VK_TRUE, UINT64_MAX);
}

void VulkanFence::Reset()
{
	XH_ASSERT(mFence.has_value())
	vkResetFences(mDevice->GetVkDevice(), 1, &mFence.value());
}

} // namespace XHuang

