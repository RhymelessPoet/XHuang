#include "VulKanDevice.h"
#include "VulkanQueue.h"
#include "VulkanSurface.h"

namespace XHuang
{
VulkanDevice::~VulkanDevice()
{
}

bool VulkanDevice::PickPhysicalDevice(const Vector<VkPhysicalDevice> &devices, VulkanSurfaceHandle surface)
{
    if (surface != nullptr) {
        mSurface = surface;
    }
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
    XH_ASSERT(mPhysicalDevice.has_value())
    GetQueueFamilyIndices(mPhysicalDevice.value(), mQueueFamilyIndices);
    Vector<const char*> Extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    Vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    float queuePriority = 1.0;
    for (auto queueFamilyIndex : mQueueFamilyIndices.Families()) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.pNext            = nullptr;
        queueCreateInfo.flags            = 0;
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount       = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    VkPhysicalDeviceFeatures requiredFeatures{};
    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount    = static_cast<UInt32>(queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos       = queueCreateInfos.data();
    deviceCreateInfo.enabledLayerCount       = 0;
    deviceCreateInfo.ppEnabledLayerNames     = nullptr;
    deviceCreateInfo.enabledExtensionCount   = static_cast<UInt32>(Extensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = Extensions.data();
    deviceCreateInfo.pEnabledFeatures        = [&](VkPhysicalDeviceFeatures& features) -> VkPhysicalDeviceFeatures*
    {
        features.samplerAnisotropy        = VK_TRUE;
        features.fragmentStoresAndAtomics = VK_TRUE;
        features.independentBlend         = VK_TRUE;
        features.geometryShader           = VK_TRUE;
        return &features;
    }(requiredFeatures);
    VkDevice device {};
    VkResult result = vkCreateDevice(mPhysicalDevice.value(), &deviceCreateInfo, nullptr, &device);
    if (result != VK_SUCCESS) {
        // log error
    }
    else {
        mLogicalDevice = device;
    }
}

void VulkanDevice::ResetLogicalDevice(VkAllocationCallbacks* allocator)
{
    vkDestroyDevice(mLogicalDevice.value(), allocator);
}

bool VulkanDevice::GetSwapchainSupportDetails(VulkanSwapchain::SupportDetails &details)
{
    if (mSurface != nullptr) {
        VkSurfaceKHR surface = mSurface->GetVkSurface();
        VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mPhysicalDevice.value(), surface, &details.Capabilities);
        if (result != VK_SUCCESS) {
            // log
            return false;
        }
        UInt32 frameCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice.value(), surface, &frameCount, nullptr);
        if (frameCount > 0) {
            result = VK_RESULT_MAX_ENUM;
            details.Formats.resize(frameCount);
            result = vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice.value(), surface, &frameCount, details.Formats.data());
        }
        if (result != VK_SUCCESS) {
            // log
            return false;
        }
        UInt32 presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(mPhysicalDevice.value(), surface, &presentModeCount, nullptr);
        if (presentModeCount > 0) {
            result = VK_RESULT_MAX_ENUM;
            details.PresentModes.resize(presentModeCount);
            result = vkGetPhysicalDeviceSurfacePresentModesKHR(mPhysicalDevice.value(), surface, &presentModeCount, details.PresentModes.data());
        }
        if (result != VK_SUCCESS) {
            // log
            return false;
        }
        details.GraphicsFamilyIndex = mQueueFamilyIndices.GraphicsFamily;
        details.PresentFamilyIndex  = mQueueFamilyIndices.PresentFamily;
        return true;
    }
    // log
    return false;
}

VulkanQueueHandle& VulkanDevice::CreateGraphicsQueue(VulkanDeviceHandle& device)
{
    device->mGraphicsQueue = MakeHandle<VulkanQueue>(device, device->mQueueFamilyIndices.GraphicsFamily.value(), 0);
    return device->mGraphicsQueue;
}

VulkanQueueHandle& VulkanDevice::CreateComputeQueue(VulkanDeviceHandle& device)
{
    device->mComputeQueue = MakeHandle<VulkanQueue>(device, device->mQueueFamilyIndices.ComputeFamily.value(), 0);
    return device->mComputeQueue;
}

VulkanQueueHandle& VulkanDevice::CreatePresentQueue(VulkanDeviceHandle& device)
{
    device->mPresentQueue = MakeHandle<VulkanQueue>(device, device->mQueueFamilyIndices.PresentFamily.value(), 0);
    return device->mPresentQueue;
}

Tuple<VkSwapchainKHR, Vector<VkImage>> VulkanDevice::CreateSwapchain(const VkSwapchainCreateInfoKHR &swapchainDesc)
{
    XH_ASSERT(mLogicalDevice.has_value())
    VkSwapchainKHR swapchain {};
    Vector<VkImage> images;
    if (vkCreateSwapchainKHR(mLogicalDevice.value(), &swapchainDesc, nullptr, &swapchain) != VK_SUCCESS) {
        // log error
    } else {
        UInt32 imageCount = 0;
        vkGetSwapchainImagesKHR(mLogicalDevice.value(), swapchain, &imageCount, nullptr);
        images.resize(imageCount);
        vkGetSwapchainImagesKHR(mLogicalDevice.value(), swapchain, &imageCount, images.data());
    }
    return std::make_tuple(swapchain, images);
}

void VulkanDevice::DestroySwapchain(VkSwapchainKHR swapchain)
{
    vkDestroySwapchainKHR(mLogicalDevice.value(), swapchain, nullptr);
}

VkImage VulkanDevice::CreateImage(const VkImageCreateInfo &imageDesc)
{
    XH_ASSERT(mLogicalDevice.has_value())
    VkImage image {};
    if (vkCreateImage(mLogicalDevice.value(), &imageDesc, nullptr, &image) != VK_SUCCESS) {
        // log error
    }
    return image;
}

void VulkanDevice::DestroyImage(VkImage image)
{
    vkDestroyImage(mLogicalDevice.value(), image, nullptr);
}

VkImageView VulkanDevice::CreateImageView(const VkImageViewCreateInfo imageViewDesc)
{
    XH_ASSERT(mLogicalDevice.has_value())
    VkImageView imageView {};
    if (vkCreateImageView(mLogicalDevice.value(), &imageViewDesc, nullptr, &imageView) != VK_SUCCESS) {
        // log error
    }
    return imageView;
}

void VulkanDevice::DestroyImageView(VkImageView imageView)
{
    vkDestroyImageView(mLogicalDevice.value(), imageView, nullptr);
}

VkFramebuffer VulkanDevice::CreateFramebuffer(const VkFramebufferCreateInfo &frameBufferDesc)
{
    XH_ASSERT(mLogicalDevice.has_value());
    VkFramebuffer frameBuffer {};
    if (vkCreateFramebuffer(mLogicalDevice.value(), &frameBufferDesc, nullptr, &frameBuffer) != VK_SUCCESS) {
        // log error
    }
    return frameBuffer;
}

void VulkanDevice::DestroyFrameBuffer(VkFramebuffer frameBuffer)
{
    vkDestroyFramebuffer(mLogicalDevice.value(), frameBuffer, nullptr);
}

VkCommandPool VulkanDevice::CreateCommandPool(EQueueType queueType)
{
    XH_ASSERT(mLogicalDevice.has_value())
    VkCommandPool commandPool {};
    if (!mQueueFamilyIndices[queueType].has_value()) {
        // log error
        return commandPool;
    }
    VkCommandPoolCreateInfo commandPoolDesc {};
    commandPoolDesc.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolDesc.pNext = nullptr;
    commandPoolDesc.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolDesc.queueFamilyIndex = mQueueFamilyIndices[queueType].value();

    if (vkCreateCommandPool(mLogicalDevice.value(), &commandPoolDesc, nullptr, &commandPool) != VK_SUCCESS) {
        // log error
    }
    return commandPool;
}

void VulkanDevice::DestroyCommandPool(VkCommandPool commandPool)
{
    vkDestroyCommandPool(mLogicalDevice.value(), commandPool, nullptr);
}

VkCommandBuffer VulkanDevice::AllocateCommandBuffer(VkCommandPool commandPool, VkCommandBufferLevel level)
{
    XH_ASSERT(mLogicalDevice.has_value())
    VkCommandBufferAllocateInfo allocateInfo {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.pNext = nullptr;
    allocateInfo.commandPool = commandPool;
    allocateInfo.level = level;
    allocateInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffer {};
    if (vkAllocateCommandBuffers(mLogicalDevice.value(), &allocateInfo, &commandBuffer) != VK_SUCCESS) {
        // log error
    }
    return commandBuffer;
}

void VulkanDevice::FreeCommandBuffer(VkCommandPool commandPool, VkCommandBuffer commandBuffer)
{
    vkFreeCommandBuffers(mLogicalDevice.value(), commandPool, 1, &commandBuffer);
}

VkSemaphore VulkanDevice::CreateSemaphore(const VkSemaphoreCreateInfo &semaphoreDesc)
{
    XH_ASSERT(mLogicalDevice.has_value())
    VkSemaphore semaphore {};
    if (vkCreateSemaphore(mLogicalDevice.value(), &semaphoreDesc, nullptr, &semaphore) != VK_SUCCESS) {
        // log error
    }
    return semaphore;
}

void VulkanDevice::DestroySemaphore(VkSemaphore semaphore)
{
    vkDestroySemaphore(mLogicalDevice.value(), semaphore, nullptr);
}

VkFence VulkanDevice::CreateFence(const VkFenceCreateInfo& fenceDesc)
{
    XH_ASSERT(mLogicalDevice.has_value())
    VkFence fence {};
    if (vkCreateFence(mLogicalDevice.value(), &fenceDesc, nullptr, &fence) != VK_SUCCESS) {
        // log error
    }
    return fence;
}

void VulkanDevice::DestroyFence(VkFence fence)
{
    vkDestroyFence(mLogicalDevice.value(), fence, nullptr);
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

        if (mSurface != nullptr && mSurface->IsComplete()) {
            VkBool32 isPresentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, idx, mSurface->GetVkSurface(), &isPresentSupport);
            if (isPresentSupport) {
                indices.PresentFamily = idx;
            }
        }
        
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
