#include "VulkanImage.h"
#include "VulKanDevice.h"

namespace XHuang
{
    
VulkanImage::~VulkanImage()
{
     if (mImage.has_value() && mOwnResource)
     {
          mDevice->DestroyImage(mImage.value());
     }
}

VkImage VulkanImage::GetVkImage() const
{
     XH_ASSERT(mImage.has_value())
     return mImage.value();
}

VulkanImage2D::VulkanImage2D(const VulkanDeviceHandle& device) : VulkanImage(device, VK_IMAGE_TYPE_2D)
{
     VkImageCreateInfo imageDesc {};
     imageDesc.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
     imageDesc.pNext = nullptr;
     imageDesc.flags = VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT;
     imageDesc.imageType = mImageType;
     imageDesc.format = VK_FORMAT_B8G8R8A8_UNORM;
     imageDesc.extent = { mExtent.X, mExtent.Y, 1u };
     imageDesc.mipLevels = 1;
     imageDesc.arrayLayers = 1;
     imageDesc.samples = VK_SAMPLE_COUNT_1_BIT;
     imageDesc.tiling = VK_IMAGE_TILING_OPTIMAL;
     imageDesc.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
     imageDesc.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
     imageDesc.queueFamilyIndexCount = 0;
     imageDesc.pQueueFamilyIndices = nullptr;

     VkImage image = mDevice->CreateImage(imageDesc);
     if (image != VK_NULL_HANDLE) {
          mImage = image;
          mOwnResource = true;
     } else {
          // log error
     }
}

VulkanImage2D::VulkanImage2D(const VulkanDeviceHandle& device, VkImage image) : VulkanImage(device, VK_IMAGE_TYPE_2D)
{
     mImage = image;
}

VulkanImage2D::~VulkanImage2D()
{
     
}
}