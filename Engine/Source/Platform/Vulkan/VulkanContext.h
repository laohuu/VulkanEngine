#ifndef ENGINE_VULKANCONTEXT_H
#define ENGINE_VULKANCONTEXT_H

#include "Renderer/RendererContext.h"

namespace Engine
{
    class VulkanContext : public RendererContext
    {
    public:
        VulkanContext();
        virtual ~VulkanContext();

        virtual void Init() override;
    };
} // namespace Engine

#endif // ENGINE_VULKANCONTEXT_H
