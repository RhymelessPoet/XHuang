#include "VulkanRHI.h"

namespace XHuang
{

VulkanRHI::VulkanRHI()
{
    mRHI_API_Version = VK_API_VERSION_1_0;
}

VulkanRHI::~VulkanRHI()
{
}

void VulkanRHI::Initialize(const RHI_InitInfo& rhiInfo)
{
    VkApplicationInfo appInfo{};
    BuildApplicationInfo(rhiInfo, appInfo);
    CreateInstance(appInfo);
    InitializePhysicalDevice();
}

void VulkanRHI::BuildApplicationInfo(const RHI_InitInfo& rhiInfo, VkApplicationInfo& outAppInfo)
{
    outAppInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    outAppInfo.pNext              = nullptr;
    outAppInfo.pApplicationName   = rhiInfo.mApplicationName.c_str();
    outAppInfo.applicationVersion = rhiInfo.mApplicationVersion;
    outAppInfo.pEngineName        = rhiInfo.mEngineName.c_str();
    outAppInfo.engineVersion      = rhiInfo.mEngineVersion;
    outAppInfo.apiVersion         = mRHI_API_Version;
}

void VulkanRHI::CreateInstance(const VkApplicationInfo& appInfo)
{
    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext                   = nullptr;
    instanceCreateInfo.flags                   = 0;
    instanceCreateInfo.pApplicationInfo        = &appInfo;
    instanceCreateInfo.enabledLayerCount       = 0;
    instanceCreateInfo.ppEnabledLayerNames     = nullptr;
    instanceCreateInfo.enabledExtensionCount   = 0;
    instanceCreateInfo.ppEnabledExtensionNames = nullptr;

    if (vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance) != VK_SUCCESS) {
        
    }
}

void VulkanRHI::InitializePhysicalDevice()
{
    UInt32 physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, nullptr);
    if (physicalDeviceCount > 0) {
        Vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, physicalDevices.data());
        for (const auto& device : physicalDevices) {
            VkPhysicalDeviceProperties physicalDeviceProperties;
            vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);
            int score = 0;
        }
    }
    else {

    }
}

} // namespace XHuang