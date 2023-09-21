#pragma once
#include "VulkanDevice.h"
#include "VulkanDebugger.h"
#include "Render/RHI/RHI.h"

namespace XHuang
{
    class VulkanRHI final : public RHI
    {
    public:
        VulkanRHI(/* args */);
        ~VulkanRHI() override;
        void Initialize(const RHI_InitInfo& rhiInfo) override;
        void Reset();

    private:
        void PopulateApplicationInfo(const RHI_InitInfo& rhiInfo, VkApplicationInfo& outAppInfo);
        bool CheckValidationLayerSupport();
        void GetRequiredExtensions(Vector<const char*>& requiredExtensions);
        void CreateInstance(const VkApplicationInfo& appInfo);
        void InitializeDevice();
        void CreateLogicalDevice();
        
    public:
        bool mEnableValidationLayers = false;
        Optional<VkInstance> mInstance;
        VulkanDevice         mDevice;
        VulkanDebugger       mDebugger;

    private:
        const Vector<String> mValidationLayers {"VK_LAYER_KHRONOS_validation"};
    };
    
}