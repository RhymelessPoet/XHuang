#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanRHI.h"
#include "Window/Window.h"

namespace XHuang
{

VulkanRHI::VulkanRHI()
{
    mRHI_API_Version = VK_API_VERSION_1_0;
    mDevice = MakeShared<VulkanDevice>();
    mDebugger = MakeShared<VulkanDebugger>();
    mSurface = MakeShared<VulkanSurface>();
    mSwapchain = MakeShared<VulkanSwapchain>();
}

VulkanRHI::~VulkanRHI()
{
}

void VulkanRHI::Initialize(const RHI_InitInfo& rhiInfo)
{
#ifndef NDEBUG
    mEnableValidationLayers = true;
#else
    mEnableValidationLayers = false;
#endif
    VkApplicationInfo appInfo{};
    PopulateApplicationInfo(rhiInfo, appInfo);
    CreateInstance(appInfo);
    if (rhiInfo.Window != nullptr) {
        mSurface->Initialize(mInstance.value(), rhiInfo.Window);
    }
    InitializeDevice();
    if (mEnableValidationLayers) {
        mDebugger->Initialize(mInstance.value());
    }
    if (mDevice->IsSupportPresent()) {
        InitializeSwapchain();
    }
}

void VulkanRHI::Reset()
{
    mDevice->ResetLogicalDevice();
    if (mEnableValidationLayers) {
        mDebugger->Reset(mInstance.value());
    }
    vkDestroyInstance(mInstance.value(), nullptr);
}

void VulkanRHI::PopulateApplicationInfo(const RHI_InitInfo& rhiInfo, VkApplicationInfo& outAppInfo)
{
    outAppInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    outAppInfo.pNext              = nullptr;
    outAppInfo.pApplicationName   = rhiInfo.ApplicationName.c_str();
    outAppInfo.applicationVersion = rhiInfo.ApplicationVersion;
    outAppInfo.pEngineName        = rhiInfo.EngineName.c_str();
    outAppInfo.engineVersion      = rhiInfo.EngineVersion;
    outAppInfo.apiVersion         = mRHI_API_Version;
}

bool VulkanRHI::CheckValidationLayerSupport()
{
    UInt32 layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    Vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    UnorderedSet<String> validationLayerSet(mValidationLayers.begin(), mValidationLayers.end());
    for (const auto& layer : availableLayers) {
        String layerName = layer.layerName;
        validationLayerSet.erase(layerName);
    }
    return validationLayerSet.empty();
}

void VulkanRHI::GetRequiredExtensions(Vector<const char *>& requiredExtensions)
{
    const char** glfwExtensions;
    UInt32 glfwExtensionCount = 0;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    requiredExtensions.reserve(glfwExtensionCount + 1);
    requiredExtensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if (mEnableValidationLayers) {
        requiredExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }
}

void VulkanRHI::CreateInstance(const VkApplicationInfo& appInfo)
{
    if (mEnableValidationLayers && !CheckValidationLayerSupport()) {
        // log
    }
    Vector<char const*> validationLayers;
    Vector<const char*> requiredExtensions;
    GetRequiredExtensions(requiredExtensions);
    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext                   = nullptr;
    instanceCreateInfo.flags                   = 0;
    instanceCreateInfo.pApplicationInfo        = &appInfo;
    if (mEnableValidationLayers) {
        auto ToCStringVector = [](const Vector<String>& layers) -> Vector<char const*>
        {
            Vector<char const*> retLayers;
            for (const auto& layer : layers) {
                retLayers.push_back(layer.c_str());
            }
            return retLayers;
        };
        VkDebugUtilsMessengerCreateInfoEXT debugMesengerCreateInfo{};
        mDebugger->PopulateDebugMessengerCreateInfo(debugMesengerCreateInfo);
        validationLayers = ToCStringVector(mValidationLayers);
        instanceCreateInfo.enabledLayerCount   = mValidationLayers.size();
        instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        instanceCreateInfo.pNext               = &debugMesengerCreateInfo;
    } else {
        instanceCreateInfo.enabledLayerCount   = 0;
        instanceCreateInfo.ppEnabledLayerNames = nullptr;
    }

    instanceCreateInfo.enabledExtensionCount   = static_cast<UInt32>(requiredExtensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
    VkInstance vkInstance{nullptr};
    if (vkCreateInstance(&instanceCreateInfo, nullptr, &vkInstance) != VK_SUCCESS) {
        // log
    }
    mInstance = vkInstance;
}

void VulkanRHI::InitializeDevice()
{
    UInt32 physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(mInstance.value(), &physicalDeviceCount, nullptr);
    if (physicalDeviceCount > 0) {
        Vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        vkEnumeratePhysicalDevices(mInstance.value(), &physicalDeviceCount, physicalDevices.data());
        if (mDevice->PickPhysicalDevice(physicalDevices, mSurface)) {
            mDevice->InitializeLogicalDevice();
        } else {
            
        }
    }
}

void VulkanRHI::InitializeSwapchain()
{
    VulkanSwapchain::SupportDetails details;
    if (mDevice->GetSwapchainSupportDetails(details)) {
        mSwapchain->Initialize(mDevice, mSurface, details);
    }
}

} // namespace XHuang