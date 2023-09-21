#include "VulKanDevice.h"
#include "VulkanQueue.h"

namespace XHuang
{
VulkanDevice::~VulkanDevice()
{
}

bool VulkanDevice::PickPhysicalDevice(const Vector<VkPhysicalDevice> &devices)
{
    Vector<VkPhysicalDevice> discreteDevices{};
    Vector<VkPhysicalDevice> integratedDevices{};
    for (const auto& device : devices) {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);
        if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            discreteDevices.push_back(device);
        } if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
            integratedDevices.push_back(device);
        }
    }
    for (const auto& classifiedDevices : { discreteDevices, integratedDevices, devices }) {
        for (const auto& device : classifiedDevices) {
            if (!IsDeviceSuitable(device)) {
                continue;
            }
            mPhysicalDevice = device;
            return true;
        }
    }
    return false;
}

void VulkanDevice::InitializeLogicalDevice()
{
    ASSERT(mPhysicalDevice.has_value())
    GetQueueFamilyIndices(mPhysicalDevice.value(), mQueueFamilyIndices);

    Vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    // float queuePriority = 1.0;
    for (UInt32 queueFamilyIndex : mQueueFamilyIndices.Families()) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.flags            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount       = 1;
        queueCreateInfo.pQueuePriorities = nullptr;
        queueCreateInfos.push_back(std::move(queueCreateInfo));
    }
    VkPhysicalDeviceFeatures requiredFeatures{};
    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount    = static_cast<UInt32>(queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos       = queueCreateInfos.data();
    deviceCreateInfo.enabledLayerCount       = 0;
    deviceCreateInfo.ppEnabledLayerNames     = nullptr;
    deviceCreateInfo.enabledExtensionCount   = 0;
    deviceCreateInfo.ppEnabledExtensionNames = nullptr;
    deviceCreateInfo.pEnabledFeatures        = [&](VkPhysicalDeviceFeatures& features) -> VkPhysicalDeviceFeatures*
    {
        features.samplerAnisotropy        = VK_TRUE;
        features.fragmentStoresAndAtomics = VK_TRUE;
        features.independentBlend         = VK_TRUE;
        features.geometryShader           = VK_TRUE;
        return &features;
    }(requiredFeatures);
    VkDevice device{ nullptr };
    VkResult result = vkCreateDevice(mPhysicalDevice.value(), &deviceCreateInfo, nullptr, &device);
    if (result != VK_SUCCESS) {
        
    }
    else {
        mLogicalDevice = device;
        mGraphicsQueue = MakeShared<VulkanQueue>(device);
        mComputeQueue  = MakeShared<VulkanQueue>(device);
        mPresentQueue  = MakeShared<VulkanQueue>(device);
        mGraphicsQueue->Initialize(mQueueFamilyIndices.GraphicsFamily.value(), 0);
        mComputeQueue->Initialize(mQueueFamilyIndices.ComputeFamily.value(), 0);
        mPresentQueue->Initialize(mQueueFamilyIndices.PresentFamily.value(), 0);
    }
}

void VulkanDevice::ResetLogicalDevice(VkAllocationCallbacks* allocator)
{
    vkDestroyDevice(mLogicalDevice.value(), allocator);
}

void VulkanDevice::GetQueueFamilyIndices(const VkPhysicalDevice &device, QueueFamilyIndices &indices)
{
    UInt32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    Vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());

    for (UInt32 idx = 0; idx < queueFamilyCount; ++idx) {
        const auto& queueFamily = queueFamilyProperties[idx];

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.GraphicsFamily = idx;
        }

        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            indices.ComputeFamily = idx;
        }

        indices.PresentFamily = queueFamilyCount - 1; //to do

        if (indices.IsComplete()) {
            break;
        }
    }
}

bool VulkanDevice::IsFullyFunctional(const VkPhysicalDevice &device)
{
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);
    return features.samplerAnisotropy && features.geometryShader;
}

bool VulkanDevice::IsQueueFamilyComplete(const VkPhysicalDevice &device)
{
    QueueFamilyIndices queueFamilyIndices;
    GetQueueFamilyIndices(device, queueFamilyIndices);
    return queueFamilyIndices.IsComplete();
}

bool VulkanDevice::IsMemoryEnough(const VkPhysicalDevice &device)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);
    constexpr UInt64 MinMemorySize = UInt64(1024) * UInt64(1024) * UInt64(1024);
    for (UInt32 idx = 0; idx < memoryProperties.memoryHeapCount; ++idx) {
        const auto& heap = memoryProperties.memoryHeaps[idx];
        if ((heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) && heap.size >= MinMemorySize) {
            return true;
        }
    }
    return false;
}

bool VulkanDevice::IsSwapChainAdequate(const VkPhysicalDevice &device)
{
    return false;
}

} // namespace XHuang
