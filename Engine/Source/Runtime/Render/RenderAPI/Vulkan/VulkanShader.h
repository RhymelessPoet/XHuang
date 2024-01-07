#pragma once
#include "Base/Base.h"
#include "VulkanForwardDeclaration.h"

namespace XHuang
{

enum class EVulkanShaderType
{
    Vertex                 = VK_SHADER_STAGE_VERTEX_BIT,
    TessellationControl    = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
    TessellationEvaluation = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
    Geometry               = VK_SHADER_STAGE_GEOMETRY_BIT,
    Fragment               = VK_SHADER_STAGE_FRAGMENT_BIT,
    Compute                = VK_SHADER_STAGE_COMPUTE_BIT,
    AllGraphics            = VK_SHADER_STAGE_ALL_GRAPHICS,
    All                    = VK_SHADER_STAGE_ALL,
    Raygen_NV              = VK_SHADER_STAGE_RAYGEN_BIT_NV,
    AnyHit_NV              = VK_SHADER_STAGE_ANY_HIT_BIT_NV,
    ClosestHit_NV          = VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV,
    Mis_NV                 = VK_SHADER_STAGE_MISS_BIT_NV,
    Intersection_NV        = VK_SHADER_STAGE_INTERSECTION_BIT_NV,
    Callable_NV            = VK_SHADER_STAGE_CALLABLE_BIT_NV,
    Task_NV                = VK_SHADER_STAGE_TASK_BIT_NV,
    Mesh_NV                = VK_SHADER_STAGE_MESH_BIT_NV,
    Max                    = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM
};

class VulkanShader
{
public:
    VulkanShader(EVulkanShaderType type, VulkanDeviceHandle device, const Vector<char>& shaderCode);
    ~VulkanShader();
    const VkShaderModule& GetShaderModule() const
    { 
        XH_ASSERT(mShaderModule.has_value())
        return mShaderModule.value();
    }
    EVulkanShaderType GetShaderType() const { return mType; }
    bool GetSpecializationInfo(VkSpecializationInfo& specializationInfo) const { return false; }

private:
    VulkanDeviceHandle       mDevice;
    Optional<VkShaderModule> mShaderModule;
    EVulkanShaderType        mType;
};

} // namespace XHuang