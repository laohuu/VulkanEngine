#ifndef ENGINE_VULKANIMGUILAYER_H
#define ENGINE_VULKANIMGUILAYER_H

#include "ImGui/ImGuiLayer.h"

#include <backends/imgui_impl_vulkan.h>
#include <imgui.h>
#include <vulkan/vulkan.h>

void check_vk_result(VkResult err);

namespace Engine
{
    class VulkanImGuiLayer : public ImGuiLayer
    {
    public:
        VulkanImGuiLayer();
        VulkanImGuiLayer(const std::string& name);
        virtual ~VulkanImGuiLayer();

        void Begin() override;
        void End() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override;

    private:
        float m_Time = 0.0f;
    };
} // namespace Engine

#endif // ENGINE_VULKANIMGUILAYER_H
