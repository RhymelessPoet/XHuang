#include <iostream>
#include "VulkanDebugger.h"

namespace XHuang
{

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cerr << "Vulakan Validation Layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

void VulkanDebugger::Initialize(VkInstance instance, const VkAllocationCallbacks* allocator)
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    PopulateDebugMessengerCreateInfo(createInfo);
    auto CreateFunc = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (CreateFunc != nullptr) {
        VkDebugUtilsMessengerEXT debugMessenger;
        CreateFunc(instance, &createInfo, allocator, &debugMessenger);
        mDebugMessenger = debugMessenger;
    } else {
        // VK_ERROR_EXTENSION_NOT_PRESENT
    }
}

void VulkanDebugger::Reset(VkInstance instance, const VkAllocationCallbacks *allocator)
{
    XH_ASSERT(mDebugMessenger.has_value())
    auto DestroyFunc = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "PFN_vkDestroyDebugUtilsMessengerEXT");
    if (DestroyFunc != nullptr) {
        DestroyFunc(instance, mDebugMessenger.value(), allocator);
        mDebugMessenger.reset();
    } else {
        // VK_ERROR_EXTENSION_NOT_PRESENT
    }
}

void VulkanDebugger::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
    createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.pNext           = nullptr;
    createInfo.flags           = 0;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | 
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData       = nullptr;
}

} // namespace XHuang