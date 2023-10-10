#include "GUILayer.h"

#include "vulkan/vulkan.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Engine
{
    GUILayer::GUILayer() : Layer("GUILayer") {}

    void GUILayer::OnUpdate(Timestep ts) { m_frame = ts.GetMilliseconds(); }

    void GUILayer::OnImGuiRender()
    {
        ImGui::Begin("Settings");
        ImGui::Text("Last render: %.3fms", m_frame);
        ImGui::End();
    }

} // namespace Engine