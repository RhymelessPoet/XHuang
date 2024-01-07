#pragma once
#include "VulkanImageView.h"
#include "VulKanDevice.h"
#include "VulkanImage.h"

namespace XHuang
{

VulkanImageView::VulkanImageView(VulkanDeviceHandle device, VulkanImageHandle image, VkFormat format, VkImageAspectFlags imageAspectFlags, VkImageViewType viewType, UInt32 layerCount, UInt32 mipLevels)
                                : mDevice(std::move(device)), mImage(std::move(image))
{
    VkImageViewCreateInfo createInfo{};
    createInfo.sType     = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.pNext     = nullptr;
    createInfo.flags     = 0;
    createInfo.image     = mImage->GetVkImage();
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

    VkImageView imageView = mDevice->CreateImageView(createInfo);

    if (imageView != VK_NULL_HANDLE) {
        mImageView = imageView;

    } else {
        // log error
    }
}
VulkanImageView::~VulkanImageView()
{
    if (mImageView.has_value()) {
        mDevice->DestroyImageView(mImageView.value());
    }
}

VkImageView VulkanImageView::GetVkImageView() const
{
    XH_ASSERT(mImageView.has_value())
    return mImageView.value();
}

} // namespace XHuang

