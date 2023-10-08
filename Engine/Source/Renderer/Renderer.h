#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include "Core/Application.h"
#include "RendererContext.h"

namespace Engine
{
    class Renderer
    {
    public:
        typedef void (*RenderCommandFn)(void*);

        static Ref<RendererContext> GetContext() { return Application::Get().GetWindow().GetRenderContext(); }

        static void Init();
        static void Shutdown();

        static void BeginFrame();
        static void EndFrame();
    };
} // namespace Engine

#endif // ENGINE_RENDERER_H
