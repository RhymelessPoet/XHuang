#pragma once
#include "VulkanForwardDeclaration.h"
#include "Base/Base.h"
#include "Render/RenderAPI/RenderAPI_Definition.h"

namespace XHuang
{

class VulkanSwapchain
{
public:
    struct SupportDetails
    {
        VkSurfaceCapabilitiesKHR   Capabilities {};
        Vector<VkSurfaceFormatKHR> Formats;
        Vector<VkPresentModeKHR>   PresentModes;
        Optional<UInt32>           GraphicsFamilyIndex;
        Optional<UInt32>           PresentFamilyIndex;
    };
    VulkanSwapchain(VulkanDeviceHandle device, VulkanSurfaceHandle surface, const SupportDetails& details);
    ~VulkanSwapchain();
    
    VkSwapchainKHR GetVkSwapchain() const { XH_ASSERT(mSwapchain.has_value()) return mSwapchain.value(); }
    VkFormat GetImageFormat() const { return mImageFormat; }
    const Extent2D& GetExtent() const { return mExtent; }
    const Vector<VulkanImageViewHandle>& GetImageViews() const { return mImageViews; }
    UInt32 GetNextImageIndex(const VulkanSemaphoreHandle& semaphoreHandle) const;

private:
    void PopulateCreateInfo(const SupportDetails& details, VkSwapchainCreateInfoKHR& outCreateInfo);
    VkSurfaceFormatKHR GetSurfaceFormat(const Vector<VkSurfaceFormatKHR>& avaliableFormats);
    VkPresentModeKHR GetPresentMode(const Vector<VkPresentModeKHR>& avaliableModes);
    VkExtent2D GetExtent(const VkSurfaceCapabilitiesKHR& capabilities);

private:
    Extent2D                      mExtent { 0, 0 };
    VulkanDeviceHandle            mDevice;
    VulkanSurfaceHandle           mSurface;
    Optional<VkSwapchainKHR>      mSwapchain;
    Vector<VulkanImageHandle>     mImages;
    Vector<VulkanImageViewHandle> mImageViews;
    VkFormat                      mImageFormat = VK_FORMAT_UNDEFINED;
};

} // namespace XHuang