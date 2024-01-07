#pragma once
#include "Base/Base.h"
#include "VulkanForwardDeclaration.h"

namespace XHuang
{
    
class VulkanImageView
{
public:
    VulkanImageView(VulkanDeviceHandle device, VulkanImageHandle image, VkFormat format, VkImageAspectFlags imageAspectFlags, VkImageViewType viewType, UInt32 layerCount, UInt32 mipLevels);
    ~VulkanImageView();
    VkImageView GetVkImageView() const;

private:
    Optional<VkImageView> mImageView;
    VulkanDeviceHandle    mDevice = nullptr;
    VulkanImageHandle     mImage = nullptr;
};

} // namespace XHuang
