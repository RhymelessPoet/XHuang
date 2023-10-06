#pragma once
#include "VulkanDevice.h"
#include "VulkanDebugger.h"
#include "VulkanSurface.h"
#include "VulkanSwapchain.h"
#include "Render/RHI/RHI.h"

namespace XHuang
{
    class VulkanRHI final : public RHI
    {
    public:
        VulkanRHI();
        ~VulkanRHI() override;
        void Initialize(const RHI_InitInfo& rhiInfo) override;
        void Reset();

    private:
        void PopulateApplicationInfo(const RHI_InitInfo& rhiInfo, VkApplicationInfo& outAppInfo);
        bool CheckValidationLayerSupport();
        void GetRequiredExtensions(Vector<const char*>& requiredExtensions);
        void CreateInstance(const VkApplicationInfo& appInfo);
        void InitializeDevice();
        void InitializeSwapchain();
        
    public:
        bool mEnableValidationLayers = false;
        Optional<VkInstance> mInstance;
        VulkanDeviceSPtr         mDevice;
        VulkanDebuggerSPtr       mDebugger;
        VulkanSurfaceSPtr        mSurface;
        VulkanSwapchainSPtr      mSwapchain;

    private:
        const Vector<String> mValidationLayers {"VK_LAYER_KHRONOS_validation"};
        bool mIsNeedToDisplay = false;
    };
    
}