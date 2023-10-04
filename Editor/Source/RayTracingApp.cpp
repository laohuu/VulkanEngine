#include "GUILayer.h"

#include <Core/EntryPoint.h>
#include <Engine.h>

namespace Engine
{
    class EngineEditorApp : public Engine::Application
    {
    public:
        EngineEditorApp(const Engine::ApplicationSpecification& spec) : Application(spec) { PushLayer(new GUILayer()); }
        ~EngineEditorApp() override = default;
    };
} // namespace Engine

Engine::Application* Engine::CreateApplication(int argc, char** argv)
{
    Engine::ApplicationSpecification spec;
    spec.Name = "Vulkan Engine";

    return new Engine::EngineEditorApp(spec);
}