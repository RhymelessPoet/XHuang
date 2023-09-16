#pragma once
#include "RHI.h"
#include <vulkan/vulkan.h>

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
        void InitializePhysicalDevice();

    public:
        VkInstance mInstance{nullptr};
    };
    
}