#ifndef ENGINE_VULKANSWAPCHAIN_H
#define ENGINE_VULKANSWAPCHAIN_H

#include "Core/Base.h"
// #include "Renderer/RenderCommandBuffer.h"
//
// #include "Vulkan.h"
// #include "VulkanDevice.h"
// #include "VulkanAllocator.h"

struct GLFWwindow;

namespace Engine
{
    class VulkanSwapChain
    {
    public:
        VulkanSwapChain() = default;

        void BeginFrame();
        void Present();

        friend class VulkanContext;
    };
} // namespace Engine

#endif // ENGINE_VULKANSWAPCHAIN_H
