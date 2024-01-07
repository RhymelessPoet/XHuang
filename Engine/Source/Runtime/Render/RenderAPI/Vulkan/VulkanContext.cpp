#include "VulkanContext.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulKanDevice.h"
#include "VulkanQueue.h"
#include "VulkanCommandBuffer.h"

namespace XHuang
{
VulkanContext::VulkanContext(VulkanDeviceHandle device, VulkanSwapchainHandle swapChainHandle, VulkanCommandBufferHandle commandBuffer) 
			  : mDevice(std::move(device))
			  , mSwapChainHandle(std::move(swapChainHandle))
			  , mCommandBuffer(std::move(commandBuffer))
{
	mGraphicsQueue = mDevice->GetGraphicsQueue();
	mPresentQueue  = mDevice->GetPresentQueue();
	mImageAvailableSemaphore = MakeHandle<VulkanSemaphore>(mDevice);
	mRenderFinishedSemaphore = MakeHandle<VulkanSemaphore>(mDevice);
	mSynchronousFence        = MakeHandle<VulkanFence>(mDevice);
}
VulkanContext::~VulkanContext()
{

}

UInt32 VulkanContext::GetNextFrameBufferIndex() const
{
	if (!mNextImageIndex.has_value()) {
		mNextImageIndex = mSwapChainHandle->GetNextImageIndex(mImageAvailableSemaphore);
	}
	return mNextImageIndex.value();
}

void VulkanContext::Synchronous()
{
	mSynchronousFence->Wait();
}

void VulkanContext::Submit()
{
	VkSemaphore waitSemaphores[] = { mImageAvailableSemaphore->GetVkSemaphore() };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore signalSemaphores[] = { mRenderFinishedSemaphore->GetVkSemaphore() };
	auto commandBuffer = mCommandBuffer->GetVkCommandBuffer();

	VkSubmitInfo submitInfo{};
	submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount   = 1;
	submitInfo.pWaitSemaphores      = waitSemaphores;
	submitInfo.pWaitDstStageMask    = waitStages;
	submitInfo.commandBufferCount   = 1;
	submitInfo.pCommandBuffers      = &commandBuffer;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores    = signalSemaphores;

	if (vkQueueSubmit(mGraphicsQueue->GetVkQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		// log error
	}
}

void VulkanContext::Swap()
{
	VkSemaphore renderFinishedSemaphores[] = { mRenderFinishedSemaphore->GetVkSemaphore() };
	auto vkSwapchain = mSwapChainHandle->GetVkSwapchain();
	VkPresentInfoKHR presentInfo {};
	presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext              = nullptr;
	presentInfo.waitSemaphoreCount = 1u;
	presentInfo.pWaitSemaphores    = renderFinishedSemaphores;
	presentInfo.swapchainCount     = 1u;
	presentInfo.pSwapchains        = &vkSwapchain;
	presentInfo.pImageIndices      = &(mNextImageIndex.value());
	presentInfo.pResults           = nullptr;

	if (vkQueuePresentKHR(mPresentQueue->GetVkQueue(), &presentInfo) != VK_SUCCESS) {
		// log error
	}
	mNextImageIndex.reset();
	mSynchronousFence->Reset();
}

} // namespace XHuang