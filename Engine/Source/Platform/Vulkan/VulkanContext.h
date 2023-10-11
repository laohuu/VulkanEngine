#ifndef ENGINE_VULKANCONTEXT_H
#define ENGINE_VULKANCONTEXT_H

#include "Renderer/Renderer.h"
#include "Renderer/RendererContext.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"

#include <vulkan/vulkan.h>

namespace Engine
{
    class VulkanContext : public RendererContext
    {
    public:
        VulkanContext();
        virtual ~VulkanContext();

        virtual void Init() override;

        Ref<VulkanDevice> GetDevice() { return m_Device; }

        static VkInstance GetInstance() { return s_VulkanInstance; }

        static Ref<VulkanContext> Get() { return Ref<VulkanContext>(Renderer::GetContext()); }
        static Ref<VulkanDevice>  GetCurrentDevice() { return Get()->GetDevice(); }

    private:
        // Devices
        Ref<VulkanPhysicalDevice> m_PhysicalDevice;
        Ref<VulkanDevice>         m_Device;

        // Vulkan instance
        inline static VkInstance s_VulkanInstance;

        VkDebugUtilsMessengerEXT m_DebugUtilsMessenger = VK_NULL_HANDLE;
        VkPipelineCache          m_PipelineCache       = nullptr;

        VulkanSwapChain m_SwapChain;

        // wait to remove
        inline static VkAllocationCallbacks* s_Allocator = nullptr;
    };
} // namespace Engine

#endif // ENGINE_VULKANCONTEXT_H
