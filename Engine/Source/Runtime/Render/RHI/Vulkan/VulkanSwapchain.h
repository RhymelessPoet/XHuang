#pragma once
#include "VkForwardDeclaration.h"
#include "Base/Base.h"

namespace XHuang
{

class VulkanSwapchain
{
public:
    struct SupportDetails
    {
        VkSurfaceCapabilitiesKHR   Capabilities;
        Vector<VkSurfaceFormatKHR> Formats;
        Vector<VkPresentModeKHR>   PresentModes;
        Optional<UInt32>           GraphicsFamilyIndex;
        Optional<UInt32>           PresentFamilyIndex;
    };
    VulkanSwapchain() = default;
    ~VulkanSwapchain();
    void Initialize(VulkanDeviceSPtr device, VulkanSurfaceSPtr surface, const SupportDetails& details);
    void Reset();

private:
    void PopulateCreateInfo(const SupportDetails& details, VkSwapchainCreateInfoKHR& outCreateInfo);
    VkSurfaceFormatKHR GetSurfaceFormat(const Vector<VkSurfaceFormatKHR>& avaliableFormats);
    VkPresentModeKHR GetPresentMode(const Vector<VkPresentModeKHR>& avaliableModes);
    VkExtent2D GetExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    static VkImageView CreateImageView(VkDevice device, VkImage& image, VkFormat format, VkImageAspectFlags imageAspectFlags, VkImageViewType viewType, UInt32 layerCount, UInt32 mipLevels);

private:
    VulkanDeviceWSPtr mDevice;
    VulkanSurfaceSPtr mSurface;
    Optional<VkSwapchainKHR> mSwapchain;
    Vector<VkImage> mImages;
    Vector<VkImageView> mImageViews;
    VkFormat mImageFormat = VK_FORMAT_UNDEFINED;
};

} // namespace XHuang