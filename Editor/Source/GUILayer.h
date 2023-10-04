#ifndef ENGINE_GUILAYER_H
#define ENGINE_GUILAYER_H

#include <Engine.h>

#include "Core/Image.h"

namespace Engine
{
    class GUILayer : public Layer
    {
    public:
        GUILayer();
        ~GUILayer() override = default;

        void OnUpdate(float ts) override;
        void OnImGuiRender() override;
    };
} // namespace Engine

#endif // ENGINE_GUILAYER_H
