#ifndef ENGINE_RENDERERCONTEXT_H
#define ENGINE_RENDERERCONTEXT_H

#include "Core/Ref.h"

namespace Engine
{
    class RendererContext : public RefCounted
    {
    public:
        RendererContext()          = default;
        virtual ~RendererContext() = default;

        virtual void Init() = 0;

        static Ref<RendererContext> Create();
    };
} // namespace Engine

#endif // ENGINE_RENDERERCONTEXT_H
