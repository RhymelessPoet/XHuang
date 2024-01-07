#pragma once
#include "Base/Base.h"
#include "VulkanForwardDeclaration.h"

namespace XHuang
{

class VulkanDebugger
{
public:
    VulkanDebugger() = default;
    ~VulkanDebugger() = default;
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void Initialize(VkInstance instance, const VkAllocationCallbacks* allocator = nullptr);
    void Reset(VkInstance instance, const VkAllocationCallbacks* allocator = nullptr);
    
private:
    Optional<VkDebugUtilsMessengerEXT> mDebugMessenger;

};

} // namespace XHuang