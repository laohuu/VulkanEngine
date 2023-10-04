#ifndef ENGINE_APPLICATION_H
#define ENGINE_APPLICATION_H

#include "ImGui/ImGuiLayer.h"
#include "LayerStack.h"

#include <GLFW/glfw3.h>

int main(int argc, char** argv);

namespace Engine
{
    struct ApplicationSpecification
    {
        std::string Name   = "Engine Application";
        uint32_t    Width  = 1280;
        uint32_t    Height = 720;
    };

    class Application
    {
    public:
        Application(const ApplicationSpecification& specification);
        virtual ~Application();

        void Init();
        void Shutdown();
        void Close();

        float GetTime();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        static Application&             Get() { return *s_Instance; }
        GLFWwindow*                     GetWindow() const { return m_WindowHandle; }
        const ApplicationSpecification& GetSpecification() const { return m_Specification; }

        void SubmitToMainThread(const std::function<void()>& function);

    private:
        void Run();

        void ExecuteMainThreadQueue();

    private:
        ApplicationSpecification m_Specification;
        GLFWwindow*              m_WindowHandle = nullptr;
        ImGuiLayer*              m_ImGuiLayer;
        bool                     m_Running   = true;
        bool                     m_Minimized = false;
        LayerStack               m_LayerStack;
        float                    m_LastFrameTime = 0.0f;

        std::vector<std::function<void()>> m_MainThreadQueue;
        std::mutex                         m_MainThreadQueueMutex;

    private:
        static Application* s_Instance;
        friend int ::main(int argc, char** argv);
    };

    // To be defined in CLIENT
    Application* CreateApplication(int argc, char** argv);
} // namespace Engine

#endif // ENGINE_APPLICATION_H
