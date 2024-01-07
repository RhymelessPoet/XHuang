#pragma once
#include "Base/Base.h"
#include "VulkanForwardDeclaration.h"
#include "VulkanSwapchain.h"

namespace XHuang
{

class VulkanDevice
{
public:
    VulkanDevice() = default;
    VulkanDevice(const VkPhysicalDevice& physicalDevice) : mPhysicalDevice(physicalDevice) {}
    ~VulkanDevice();
    bool PickPhysicalDevice(const Vector<VkPhysicalDevice>& devices, VulkanSurfaceHandle surface = nullptr);
    void InitializeLogicalDevice();
    void ResetLogicalDevice(VkAllocationCallbacks* allocator = nullptr);
    bool GetSwapchainSupportDetails(VulkanSwapchain::SupportDetails& details);
    VkDevice GetVkDevice() const { return mLogicalDevice.has_value() ? mLogicalDevice.value() : VK_NULL_HANDLE; }
    bool IsSupportPresent() const { return mQueueFamilyIndices.PresentFamily.has_value(); }

    const VulkanQueueHandle& GetGraphicsQueue() const { return mGraphicsQueue; }
    const VulkanQueueHandle& GetComputeQueue() const { return mComputeQueue; }
    const VulkanQueueHandle& GetPresentQueue() const { return mPresentQueue; }

    static VulkanQueueHandle& CreateGraphicsQueue(VulkanDeviceHandle& device);
    static VulkanQueueHandle& CreateComputeQueue(VulkanDeviceHandle& device);
    static VulkanQueueHandle& CreatePresentQueue(VulkanDeviceHandle& device);

    Tuple<VkSwapchainKHR, Vector<VkImage>> CreateSwapchain(const VkSwapchainCreateInfoKHR& swapchainDesc);
    void DestroySwapchain(VkSwapchainKHR swapchain);

    VkImage CreateImage(const VkImageCreateInfo& imageDesc);
    void DestroyImage(VkImage image);
    VkImageView CreateImageView(const VkImageViewCreateInfo imageViewDesc);
    void DestroyImageView(VkImageView imageView);
    VkFramebuffer CreateFramebuffer(const VkFramebufferCreateInfo& frameBufferDesc);
    void DestroyFrameBuffer(VkFramebuffer frameBuffer);

    VkCommandPool CreateCommandPool(EQueueType queueType);
    void DestroyCommandPool(VkCommandPool commandPool);
    VkCommandBuffer AllocateCommandBuffer(VkCommandPool commandPool, VkCommandBufferLevel level);
    void FreeCommandBuffer(VkCommandPool commandPool, VkCommandBuffer commandBuffer);

    VkSemaphore CreateSemaphore(const VkSemaphoreCreateInfo& semaphoreDesc);
    void DestroySemaphore(VkSemaphore semaphore);
    VkFence CreateFence(const VkFenceCreateInfo& fenceDesc);
    void DestroyFence(VkFence fence);

private:
    struct QueueFamilyIndices
    {
        static constexpr UInt32 NumOfFamily = 3;
        static constexpr Optional<UInt32> InValid {};
        Optional<UInt32> GraphicsFamily;
        Optional<UInt32> PresentFamily;
        Optional<UInt32> ComputeFamily;
        bool IsPresent = true;
        const Optional<UInt32>& operator[] (EQueueType queueType) const 
        {
            switch (queueType)
            {
            case EQueueType::Graphics:
                return GraphicsFamily;
            case EQueueType::Compute:
                return ComputeFamily;
            case EQueueType::Present:
                return PresentFamily;
            default:
                return InValid;
            }
        };
        Vector<UInt32> Families()
        {
            Set<UInt32> families;
            if (GraphicsFamily.has_value()) {
                families.insert(GraphicsFamily.value());
            }
            if (ComputeFamily.has_value()) {
                families.insert(ComputeFamily.value());
            }
            if (PresentFamily.has_value()) {
                families.insert(PresentFamily.value());
            }
             
            return Vector<UInt32>(families.begin(), families.end());
        }
        bool IsComplete()
        {
            return GraphicsFamily.has_value() && (!IsPresent || PresentFamily.has_value()) && ComputeFamily.has_value();
        }
    };
    void GetQueueFamilyIndices(const VkPhysicalDevice& device, QueueFamilyIndices& indices);
    // Physical Device Checking
    bool IsDeviceSuitable(const VkPhysicalDevice& device)
    {
        return IsFullyFunctional(device) && IsQueueFamilyComplete(device) && IsMemoryEnough(device);
    }
    bool IsFullyFunctional(const VkPhysicalDevice& device);
    bool IsQueueFamilyComplete(const VkPhysicalDevice& device);
    bool IsMemoryEnough(const VkPhysicalDevice& device);
    bool IsSwapChainAdequate(const VkPhysicalDevice& device);
    
    bool IsComplete() const { return mPhysicalDevice.has_value() && mLogicalDevice.has_value(); }
    
private:
    VulkanSurfaceHandle        mSurface;
    Optional<VkPhysicalDevice> mPhysicalDevice;
    Optional<VkDevice>         mLogicalDevice;
    QueueFamilyIndices         mQueueFamilyIndices;
    VulkanQueueHandle          mGraphicsQueue = nullptr;
    VulkanQueueHandle          mComputeQueue  = nullptr;
    VulkanQueueHandle          mPresentQueue  = nullptr;
    // Map<UInt32, UInt32>        mQueueCountInFamily;
};
} //namespace XHuang