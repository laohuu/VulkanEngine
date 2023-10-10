#ifndef ENGINE_APPLICATION_H
#define ENGINE_APPLICATION_H

#include "Core/Base.h"
#include "Core/LayerStack.h"
#include "Core/TimeStep.h"
#include "Core/Timer.h"
#include "Core/Window.h"

#include "Core/Events/ApplicationEvent.h"

#include "ImGui/ImGuiLayer.h"

#include <GLFW/glfw3.h>
#include <queue>

int main(int argc, char** argv);

namespace Engine
{
    struct ApplicationSpecification
    {
        std::string Name        = "Engine Application";
        uint32_t    WindowWidth = 1600, WindowHeight = 900;
        bool        WindowDecorated = false;
        bool        Fullscreen      = false;
        bool        VSync           = true;
        std::string WorkingDirectory;
        bool        StartMaximized = true;
        bool        Resizable      = true;
        bool        EnableImGui    = true;
    };

    class Application
    {
        using EventCallbackFn = std::function<void(Event&)>;

    public:
        Application(const ApplicationSpecification& specification);
        virtual ~Application();

        void Run();
        void Close();

        virtual void OnInit() {}
        virtual void OnShutdown();
        virtual void OnUpdate(Timestep ts) {}

        virtual void OnEvent(Event& event);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);
        void RenderImGui();

        void AddEventCallback(const EventCallbackFn& eventCallback) { m_EventCallbacks.push_back(eventCallback); }

        void SetShowStats(bool show) { m_ShowStats = show; }

        template<typename Func>
        void QueueEvent(Func&& func)
        {
            m_EventQueue.push(func);
        }

        /// Creates & Dispatches an event either immediately, or adds it to an event queue which will be proccessed at
        /// the end of each frame
        template<typename TEvent, bool DispatchImmediately = false, typename... TEventArgs>
        void DispatchEvent(TEventArgs&&... args)
        {
            static_assert(std::is_assignable_v<Event, TEvent>);

            std::shared_ptr<TEvent> event = std::make_shared<TEvent>(std::forward<TEventArgs>(args)...);
            if constexpr (DispatchImmediately)
            {
                OnEvent(*event);
            }
            else
            {
                std::scoped_lock<std::mutex> lock(m_EventQueueMutex);
                m_EventQueue.push([event]() { Application::Get().OnEvent(*event); });
            }
        }

        static Application& Get() { return *s_Instance; }
        inline Window&      GetWindow() { return *m_Window; }

        Timestep GetTimestep() const { return m_TimeStep; }
        Timestep GetFrametime() const { return m_Frametime; }

        static const char* GetConfigurationName();
        static const char* GetPlatformName();

        const ApplicationSpecification& GetSpecification() const { return m_Specification; }

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        uint32_t GetCurrentFrameIndex() const { return m_CurrentFrameIndex; }

        static bool IsRuntime() { return s_IsRuntime; }

    private:
        void ProcessEvents();

        bool OnWindowResize(WindowResizeEvent& e);
        bool OnWindowMinimize(WindowMinimizeEvent& e);
        bool OnWindowClose(WindowCloseEvent& e);

    private:
        std::unique_ptr<Window>  m_Window;
        ApplicationSpecification m_Specification;
        bool                     m_Running = true, m_Minimized = false;
        LayerStack               m_LayerStack;
        ImGuiLayer*              m_ImGuiLayer;
        Timestep                 m_Frametime;
        Timestep                 m_TimeStep;
        bool                     m_ShowStats = true;

        std::mutex                        m_EventQueueMutex;
        std::queue<std::function<void()>> m_EventQueue;
        std::vector<EventCallbackFn>      m_EventCallbacks;

        float    m_LastFrameTime     = 0.0f;
        uint32_t m_CurrentFrameIndex = 0;

        static Application* s_Instance;

        friend int ::main(int argc, char** argv);
        friend class Renderer;

    protected:
        inline static bool s_IsRuntime = false;
    };

    // To be defined in CLIENT
    Application* CreateApplication(int argc, char** argv);
} // namespace Engine

#endif // ENGINE_APPLICATION_H
