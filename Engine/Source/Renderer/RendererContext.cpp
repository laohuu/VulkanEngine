#include "RendererContext.h"

#include "Platform/Vulkan/VulkanContext.h"
namespace Engine
{
    Ref<RendererContext> RendererContext::Create() { return Ref<VulkanContext>::Create(); }
} // namespace Engine