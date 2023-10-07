#ifndef ENGINE_APPLICATION_H
#define ENGINE_APPLICATION_H

#include "Core/Base.h"
#include "Core/LayerStack.h"
#include "Core/Timer.h"
#include "Core/Timestep.h"
#include "Core/Window.h"

#include "Core/Events/ApplicationEvent.h"

#include "ImGui/ImGuiLayer.h"

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
        using EventCallbackFn = std::function<void(Event&)>;

    public:
        Application(const ApplicationSpecification& specification);
        virtual ~Application();

        void Init();
        void Shutdown();
        void Close();

        float GetTime();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        virtual void OnEvent(Event& event);

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        static Application&             Get() { return *s_Instance; }
        GLFWwindow*                     GetWindow() const { return m_WindowHandle; }
        const ApplicationSpecification& GetSpecification() const { return m_Specification; }

        void SubmitToMainThread(const std::function<void()>& function);

    private:
        void Run();

        void ExecuteMainThreadQueue();

        bool OnWindowResize(WindowResizeEvent& e);
        bool OnWindowMinimize(WindowMinimizeEvent& e);
        bool OnWindowClose(WindowCloseEvent& e);

    private:
        std::unique_ptr<Window>  m_Window;
        ApplicationSpecification m_Specification;
        GLFWwindow*              m_WindowHandle = nullptr;
        ImGuiLayer*              m_ImGuiLayer;
        Timestep                 m_Frametime;
        Timestep                 m_TimeStep;
        bool                     m_Running   = true;
        bool                     m_Minimized = false;
        LayerStack               m_LayerStack;
        float                    m_LastFrameTime = 0.0f;

        std::vector<std::function<void()>> m_MainThreadQueue;
        std::mutex                         m_MainThreadQueueMutex;

        std::vector<EventCallbackFn> m_EventCallbacks;

    private:
        static Application* s_Instance;
        friend int ::main(int argc, char** argv);
    };

    // To be defined in CLIENT
    Application* CreateApplication(int argc, char** argv);
} // namespace Engine

#endif // ENGINE_APPLICATION_H
