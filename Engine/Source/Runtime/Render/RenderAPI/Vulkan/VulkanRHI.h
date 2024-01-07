#pragma once
#include "VulkanDevice.h"
#include "VulkanDebugger.h"
#include "VulkanSurface.h"
#include "VulkanSwapchain.h"
#include "Render/RenderAPI/RHI.h"

namespace XHuang
{
    class VulkanRHI final : public RHI
    {
    public:
        VulkanRHI();
        ~VulkanRHI() override;
        void Initialize(const RHI_InitInfo& rhiInfo) override;
        void Reset();
        IRenderContextHandle MakeContext() override;

    private:
        void PopulateApplicationInfo(const RHI_InitInfo& rhiInfo, VkApplicationInfo& outAppInfo);
        bool CheckValidationLayerSupport();
        void GetRequiredExtensions(Vector<const char*>& requiredExtensions);
        void CreateInstance(const VkApplicationInfo& appInfo);
        void InitializeDevice();
        void InitializeSwapchain();
        
    public:
        bool                    mEnableValidationLayers = false;
        Optional<VkInstance>    mInstance;
        VulkanDeviceHandle      mDevice;
        VulkanDebuggerHandle    mDebugger;
        VulkanSurfaceHandle     mSurface;
        VulkanSwapchainHandle   mSwapchain;
        VulkanCommandPoolHandle mCommandPool;

    private:
        const Vector<String> mValidationLayers {"VK_LAYER_KHRONOS_validation"};
        bool                 mIsNeedToDisplay = false;
    };
    
}