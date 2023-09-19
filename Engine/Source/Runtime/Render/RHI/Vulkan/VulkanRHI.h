#pragma once
#include "VulKanDevice.h"
#include "Render/RHI/RHI.h"

namespace XHuang
{
    class VulkanRHI final : public RHI
    {
    public:
        VulkanRHI(/* args */);
        ~VulkanRHI() override;
        void Initialize(const RHI_InitInfo& rhiInfo) override;

    private:
        void BuildApplicationInfo(const RHI_InitInfo& rhiInfo, VkApplicationInfo& outAppInfo);
        void CreateInstance(const VkApplicationInfo& appInfo);
        void InitializeDevice();
        void CreateLogicalDevice();
        
    public:
        VkInstance      mInstance{nullptr};
        VulkanDevice    mDevice;
    };
    
}