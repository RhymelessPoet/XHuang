#include "VulkanSemaphore.h"
#include "VulKanDevice.h"

XHuang::VulkanSemaphore::VulkanSemaphore(VulkanDeviceHandle device) : mDevice(std::move(device))
{
    VkSemaphoreCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VkSemaphore semaphore = mDevice->CreateSemaphore(createInfo);
    if (semaphore != VK_NULL_HANDLE) {
        mSemaphore = semaphore;
    } else {
        // log error
    }
}

XHuang::VulkanSemaphore::~VulkanSemaphore()
{
    if (mSemaphore.has_value()) {
        mDevice->DestroySemaphore(mSemaphore.value());
    }
}

VkSemaphore XHuang::VulkanSemaphore::GetVkSemaphore() const
{
    XH_ASSERT(mSemaphore.has_value())
    return mSemaphore.value();
}
