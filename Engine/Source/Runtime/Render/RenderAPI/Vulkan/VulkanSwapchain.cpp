#include "VulkanSwapchain.h"
#include "VulKanDevice.h"
#include "VulkanSurface.h"
#include "VulkanImageView.h"
#include "VulkanImage.h"
#include "VulkanSemaphore.h"
#include "Math/Math.h"

namespace XHuang
{
    
VulkanSwapchain::VulkanSwapchain(VulkanDeviceHandle device, VulkanSurfaceHandle surface, const SupportDetails &details)
{
    mDevice = device;
    mSurface = surface;
    VkSwapchainCreateInfoKHR createInfo{};
    PopulateCreateInfo(details, createInfo);
    mImageFormat = createInfo.imageFormat;
    auto swapchainWithImages = mDevice->CreateSwapchain(createInfo);
    if (std::get<0>(swapchainWithImages) == VK_NULL_HANDLE) {
        // log
    } else {
        mSwapchain = std::get<0>(swapchainWithImages);
        auto& images = std::get<1>(swapchainWithImages);
        UInt32 imageCount = images.size();
        mImages.resize(imageCount);
        mImageViews.resize(imageCount);
        for (UInt32 index = 0; index < imageCount; ++index) {
            mImages[index] = MakeHandle<VulkanImage2D>(mDevice, images[index]);
            mImageViews[index] = MakeHandle<VulkanImageView>(mDevice, mImages[index], mImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D, 1u, 1u);
        }
    }
}

VulkanSwapchain::~VulkanSwapchain()
{
    if (mSwapchain.has_value()) {
        mDevice->DestroySwapchain(mSwapchain.value());
    }
}

void VulkanSwapchain::PopulateCreateInfo(const SupportDetails &details, VkSwapchainCreateInfoKHR& outCreateInfo)
{
    UInt32 imageCount = details.Capabilities.maxImageCount < 3 ? details.Capabilities.maxImageCount : 3;
    VkSurfaceFormatKHR surfaceFormat = GetSurfaceFormat(details.Formats);
    outCreateInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    outCreateInfo.pNext            = nullptr;
    outCreateInfo.flags            = 0;
    outCreateInfo.surface          = mSurface->GetVkSurface();
    outCreateInfo.minImageCount    = imageCount;
    outCreateInfo.imageFormat      = surfaceFormat.format;
    outCreateInfo.imageColorSpace  = surfaceFormat.colorSpace;
    outCreateInfo.imageExtent      = GetExtent(details.Capabilities);
    outCreateInfo.imageArrayLayers = 1;
    outCreateInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (details.GraphicsFamilyIndex.has_value() && details.PresentFamilyIndex.has_value() && details.GraphicsFamilyIndex != details.PresentFamilyIndex) {
        UInt32 familyIndices[] = { details.GraphicsFamilyIndex.value(), details.PresentFamilyIndex.value() }; 
        outCreateInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        outCreateInfo.queueFamilyIndexCount = 2;
        outCreateInfo.pQueueFamilyIndices   = familyIndices;
    } else {
        outCreateInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
    }
    outCreateInfo.preTransform     = details.Capabilities.currentTransform;
    outCreateInfo.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    outCreateInfo.presentMode      = GetPresentMode(details.PresentModes);
    outCreateInfo.clipped          = VK_TRUE;
    outCreateInfo.oldSwapchain     = VK_NULL_HANDLE;
    mExtent.X = outCreateInfo.imageExtent.width;
    mExtent.Y = outCreateInfo.imageExtent.height;
}

VkSurfaceFormatKHR VulkanSwapchain::GetSurfaceFormat(const Vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& format : availableFormats) {
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }
    return availableFormats.empty() ? VkSurfaceFormatKHR() : availableFormats[0];
}

VkPresentModeKHR VulkanSwapchain::GetPresentMode(const Vector<VkPresentModeKHR>& availableModes)
{
    for (const auto& presentMode : availableModes) {
        if (VK_PRESENT_MODE_MAILBOX_KHR == presentMode) {
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapchain::GetExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    UInt32 width = mSurface->GetWindow()->GetWindowWidth();
    UInt32 height = mSurface->GetWindow()->GetWindowHeight();
    if (capabilities.currentExtent.width == UINT32_MAX) {
        width = Math::Clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.height);
        height = Math::Clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.width);
    } else {
        return capabilities.currentExtent;
    }
    return {width, height};
}

UInt32 VulkanSwapchain::GetNextImageIndex(const VulkanSemaphoreHandle& semaphoreHandle) const
{
    XH_ASSERT(mSwapchain.has_value())
    UInt32 nextImageIndex = 0;
    auto result = vkAcquireNextImageKHR(mDevice->GetVkDevice(), mSwapchain.value(), UINT64_MAX, semaphoreHandle->GetVkSemaphore(), VK_NULL_HANDLE, &nextImageIndex);
    if (result != VK_SUCCESS) {
        // log error
    }
    return nextImageIndex;
}

} // namespace XHuang