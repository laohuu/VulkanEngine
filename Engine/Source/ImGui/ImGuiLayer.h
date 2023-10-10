#ifndef ENGINE_IMGUILAYER_H
#define ENGINE_IMGUILAYER_H

#include "Core/Layer.h"

namespace Engine
{
    class ImGuiLayer : public Layer
    {
    public:
        virtual void Begin() = 0;
        virtual void End()   = 0;

        void SetDarkThemeColors();
        void SetDarkThemeV2Colors();

        void AllowInputEvents(bool allowEvents);

        static ImGuiLayer* Create();
    };
} // namespace Engine

#endif // ENGINE_IMGUILAYER_H
