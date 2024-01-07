#include "VulkanForwardDeclaration.h"
#include "Base/Base.h"

namespace XHuang
{

class VulkanPipelineLayout
{
public:
    VulkanPipelineLayout(const VulkanDeviceHandle& device);
    ~VulkanPipelineLayout();
    VkPipelineLayout GetVkLayout() const { return mLayout.value(); }

private:
    VulkanDeviceHandle         mDevice;
    Optional<VkPipelineLayout> mLayout;
};

} // namespace XHuang