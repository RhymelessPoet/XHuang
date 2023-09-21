#pragma once
#include "Base/Base.h"
#include "VkForwardDeclaration.h"

namespace XHuang
{

class VulkanDevice
{
public:
    VulkanDevice() = default;
    VulkanDevice(const VkPhysicalDevice& physicalDevice) : mPhysicalDevice(physicalDevice) {}
    ~VulkanDevice();
    bool PickPhysicalDevice(const Vector<VkPhysicalDevice>& devices);
    void InitializeLogicalDevice();
    void ResetLogicalDevice(VkAllocationCallbacks* allocator = nullptr);

private:
    struct QueueFamilyIndices
    {
        static constexpr UInt32 NumOfFamily = 3;
        Optional<UInt32> GraphicsFamily;
        Optional<UInt32> PresentFamily;
        Optional<UInt32> ComputeFamily;
        Vector<UInt32> Families() { return { GraphicsFamily.value(), PresentFamily.value(), ComputeFamily.value() }; }
        bool IsComplete()
        {
            return GraphicsFamily.has_value() && PresentFamily.has_value() && ComputeFamily.has_value();
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
    Optional<VkPhysicalDevice> mPhysicalDevice;
    Optional<VkDevice>         mLogicalDevice;
    QueueFamilyIndices         mQueueFamilyIndices;
    VulkanQueueSPtr            mGraphicsQueue = nullptr;
    VulkanQueueSPtr            mComputeQueue  = nullptr;
    VulkanQueueSPtr            mPresentQueue  = nullptr;
    // Map<UInt32, UInt32>        mQueueCountInFamily;
};
} //namespace XHuang