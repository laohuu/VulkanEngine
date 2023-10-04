#ifndef ENGINE_IMGUILAYER_H
#define ENGINE_IMGUILAYER_H

#include "Core/Layer.h"
#include <imgui.h>

#include <backends/imgui_impl_vulkan.h>
#include <vulkan/vulkan.h>

void check_vk_result(VkResult err);

namespace Engine
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override = default;

        void OnAttach() override;
        void OnDetach() override;

        void Begin();
        void End();

        void BlockEvents(bool block) { m_BlockEvents = block; }

        void SetDarkThemeColors();

        static uint32_t GetActiveWidgetID();

        static VkInstance               GetInstance();
        static VkPhysicalDevice         GetPhysicalDevice();
        static VkDevice                 GetDevice();
        static VkAllocationCallbacks*   GetAllocator();
        static ImGui_ImplVulkanH_Window GetMainWindowData();
        static VkQueue                  GetQueue();

    private:
        bool m_BlockEvents = true;
    };
} // namespace Engine

#endif // ENGINE_IMGUILAYER_H
