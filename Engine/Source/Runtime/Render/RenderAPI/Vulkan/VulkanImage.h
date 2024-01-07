#pragma once
#include "VulkanForwardDeclaration.h"
#include "Base/Base.h"
#include "Render/RenderAPI/RenderAPI_Definition.h"

namespace XHuang
{

class VulkanImage
{
public:
    VulkanImage() = default;
    VulkanImage(const VulkanDeviceHandle& device, VkImageType imageType) : mDevice(device), mImageType(imageType) {}
    virtual ~VulkanImage();
    VkImageType GetVkImageType() const { return mImageType; }
    VkImage GetVkImage() const;

protected:
    bool               mOwnResource = false;
    VulkanDeviceHandle mDevice = nullptr;
    VkImageType        mImageType = VK_IMAGE_TYPE_MAX_ENUM;
    Optional<VkImage>  mImage;
};

class VulkanImage2D final : public VulkanImage
{
public:
    explicit VulkanImage2D(const VulkanDeviceHandle &device);
    explicit VulkanImage2D(const VulkanDeviceHandle& device, VkImage image);
    VulkanImage2D(const VulkanImage2D& other) = delete;
    ~VulkanImage2D() override;
    
private:
    Extent2D mExtent { 0, 0 };
};

} // namespace XHuang