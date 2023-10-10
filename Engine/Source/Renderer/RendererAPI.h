#ifndef ENGINE_RENDERERAPI_H
#define ENGINE_RENDERERAPI_H

namespace Engine
{
    enum class RendererAPIType
    {
        None,
        Vulkan
    };

    class RendererAPI
    {
    public:
        static RendererAPIType Current() { return s_CurrentRendererAPI; }
        static void            SetAPI(RendererAPIType api);

    private:
        inline static RendererAPIType s_CurrentRendererAPI = RendererAPIType::Vulkan;
    };
} // namespace Engine

#endif // ENGINE_RENDERERAPI_H
