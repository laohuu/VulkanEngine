#ifndef ENGINE_VULKANSWAPCHAIN_H
#define ENGINE_VULKANSWAPCHAIN_H

#include "Core/Base.h"

#include "Vulkan.h"
#include "VulkanDevice.h"
// #include "VulkanAllocator.h"

struct GLFWwindow;

namespace Engine
{
    class VulkanSwapChain
    {
    public:
        VulkanSwapChain() = default;

        void Init(VkInstance instance, const Ref<VulkanDevice>& device);
        void InitSurface(GLFWwindow* windowHandle);
        void Create(uint32_t* width, uint32_t* height, bool vsync);
        void Destroy();

        void OnResize(uint32_t width, uint32_t height);

        uint32_t GetImageCount() const { return m_ImageCount; }

        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }

        VkRenderPass GetRenderPass() { return m_RenderPass; }

        void BeginFrame();
        void Present();

    private:
        uint32_t AcquireNextImage();

        void FindImageFormatAndColorSpace();

    private:
        VkInstance        m_Instance = nullptr;
        Ref<VulkanDevice> m_Device;
        bool              m_VSync = false;

        VkFormat        m_ColorFormat;
        VkColorSpaceKHR m_ColorSpace;

        VkSwapchainKHR       m_SwapChain  = nullptr;
        uint32_t             m_ImageCount = 0;
        std::vector<VkImage> m_VulkanImages;
        struct SwapchainImage
        {
            VkImage     Image     = nullptr;
            VkImageView ImageView = nullptr;
        };
        std::vector<SwapchainImage> m_Images;

        struct
        {
            VkImage Image = nullptr;
            //            VmaAllocation MemoryAlloc = nullptr;
            VkImageView ImageView = nullptr;
        } m_DepthStencil;

        std::vector<VkFramebuffer> m_Framebuffers;

        struct SwapchainCommandBuffer
        {
            VkCommandPool   CommandPool   = nullptr;
            VkCommandBuffer CommandBuffer = nullptr;
        };
        std::vector<SwapchainCommandBuffer> m_CommandBuffers;

        struct
        {
            // Swap chain
            VkSemaphore PresentComplete = nullptr;
            // Command buffer
            VkSemaphore RenderComplete = nullptr;
        } m_Semaphores;
        VkSubmitInfo m_SubmitInfo;

        std::vector<VkFence> m_WaitFences;

        VkRenderPass m_RenderPass         = nullptr;
        uint32_t     m_CurrentBufferIndex = 0;
        uint32_t     m_CurrentImageIndex  = 0;

        uint32_t m_QueueNodeIndex = UINT32_MAX;
        uint32_t m_Width = 0, m_Height = 0;

        VkSurfaceKHR m_Surface;

        friend class VulkanContext;
    };
} // namespace Engine

#endif // ENGINE_VULKANSWAPCHAIN_H
