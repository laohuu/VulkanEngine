#include "GUILayer.h"

#include "vulkan/vulkan.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Engine
{
    GUILayer::GUILayer() : Layer("GUILayer") {}

    void GUILayer::OnUpdate(Timestep ts) {}

    void GUILayer::OnImGuiRender()
    {
        ImGui::Begin("Settings");
        ImGui::Text("Last render: %.3fms", 0.005f);
        ImGui::End();
    }

} // namespace Engine