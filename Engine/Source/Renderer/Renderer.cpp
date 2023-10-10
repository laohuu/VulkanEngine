#include "Renderer.h"

#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "RendererAPI.h"

namespace Engine
{
    static RendererAPI* s_RendererAPI = nullptr;

    void RendererAPI::SetAPI(RendererAPIType api) { s_CurrentRendererAPI = api; }

    static RendererAPI* InitRendererAPI()
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::Vulkan:
                return new VulkanRendererAPI();
            default:
                break;
        }
        return nullptr;
    }

    void Renderer::Init() { s_RendererAPI = InitRendererAPI(); }
    void Renderer::Shutdown() {}
    void Renderer::BeginFrame() {}
    void Renderer::EndFrame() {}
} // namespace Engine