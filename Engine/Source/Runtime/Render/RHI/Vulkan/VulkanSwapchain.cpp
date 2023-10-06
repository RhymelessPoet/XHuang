#include "VulkanSwapchain.h"
#include "VulKanDevice.h"
#include "VulkanSurface.h"
#include "Math/Math.h"

namespace XHuang
{
    
VulkanSwapchain::~VulkanSwapchain()
{
}

void VulkanSwapchain::Initialize(VulkanDeviceSPtr device, VulkanSurfaceSPtr surface, const SupportDetails &details)
{
    mDevice = device;
    mSurface = surface;
    VkSwapchainCreateInfoKHR createInfo{};
    PopulateCreateInfo(details, createInfo);
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    VkDevice vkDevice = mDevice.lock()->GetVkDevice();
    VkResult result = vkCreateSwapchainKHR(vkDevice, &createInfo, nullptr, &swapchain);
    if (result != VK_SUCCESS) {
        // log
    } else {
        mSwapchain = swapchain;
        UInt32 imageCount = 0;
        vkGetSwapchainImagesKHR(vkDevice, swapchain, &imageCount, nullptr);
        mImages.reserve(imageCount);
        mImages.resize(imageCount);
        vkGetSwapchainImagesKHR(vkDevice, swapchain, &imageCount, mImages.data());
        mImageFormat = createInfo.imageFormat;

        mImageViews.reserve(imageCount);
        for (UInt16 index = 0; index < imageCount; ++index) {
            VkImageView imageView = CreateImageView(vkDevice, mImages[index], mImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D, 1u, 1u);
            mImageViews.push_back(imageView);
        }
    }
}

void VulkanSwapchain::Reset()
{
    VkDevice vkDevice = mDevice.lock()->GetVkDevice();
    for (auto imageView : mImageViews) {
        vkDestroyImageView(vkDevice, imageView, nullptr);
    }
    mImageViews.clear();
    if (mSwapchain.has_value()) {
        vkDestroySwapchainKHR(vkDevice, mSwapchain.value(), nullptr);
        mSwapchain.reset();
    }
}

void VulkanSwapchain::PopulateCreateInfo(const SupportDetails &details, VkSwapchainCreateInfoKHR& outCreateInfo)
{
    UInt32 imageCount = details.Capabilities.maxImageCount < 3 ? details.Capabilities.maxImageCount : 3;
    VkSurfaceFormatKHR surfaceFormat = GetSurfaceFormat(details.Formats);
    outCreateInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    outCreateInfo.pNext            = nullptr;
    outCreateInfo.flags            = 0;
    outCreateInfo.surface          = mSurface->GetSurface();
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

VkImageView VulkanSwapchain::CreateImageView(VkDevice device, VkImage& image, VkFormat format, VkImageAspectFlags imageAspectFlags, VkImageViewType viewType, UInt32 layerCount, UInt32 mipLevels)
{
    VkImageView imageView = VK_NULL_HANDLE;
    VkImageViewCreateInfo createInfo{};
    createInfo.sType     = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.pNext     = nullptr;
    createInfo.flags     = 0;
    createInfo.image     = image;
    createInfo.viewType  = viewType;
    createInfo.format    = format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask     = imageAspectFlags;
    createInfo.subresourceRange.baseMipLevel   = 0;
    createInfo.subresourceRange.levelCount     = mipLevels;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount     = layerCount;
    if (vkCreateImageView(device, &createInfo, nullptr, &imageView) != VK_SUCCESS) {
        // log
        return imageView;
    }
    return imageView;
}

} // namespace XHuang