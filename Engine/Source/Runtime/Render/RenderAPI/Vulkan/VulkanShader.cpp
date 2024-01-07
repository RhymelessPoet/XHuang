#include "VulkanShader.h"
#include "VulKanDevice.h"

namespace XHuang
{
VulkanShader::VulkanShader(EVulkanShaderType type, VulkanDeviceHandle device, const Vector<char>& shaderCode)
{
    mType = type;
    mDevice = device;
    VkShaderModuleCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const UInt32*>(shaderCode.data());
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device->GetVkDevice(), &createInfo, nullptr, &shaderModule) == VK_SUCCESS) {
        mShaderModule = shaderModule;
    } else {
        // log
    }
}

VulkanShader::~VulkanShader()
{
    if (mShaderModule.has_value()) {
        vkDestroyShaderModule(mDevice->GetVkDevice(), mShaderModule.value(), nullptr);
        mShaderModule.reset();
    }
}
} // namespace XHuang