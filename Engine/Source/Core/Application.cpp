#include "Application.h"

#include "Input.h"
#include "Renderer/Renderer.h"

extern bool g_ApplicationRunning;

namespace Engine
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification& specification) : m_Specification(specification)
    {
        s_Instance = this;

        WindowSpecification windowSpec;
        windowSpec.Title      = specification.Name;
        windowSpec.Width      = specification.WindowWidth;
        windowSpec.Height     = specification.WindowHeight;
        windowSpec.Decorated  = specification.WindowDecorated;
        windowSpec.Fullscreen = specification.Fullscreen;
        windowSpec.VSync      = specification.VSync;
        m_Window              = std::unique_ptr<Window>(Window::Create(windowSpec));
        m_Window->Init();
        m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

        //         Init renderer and execute command queue to compile all shaders
        //        Renderer::Init();

        if (specification.StartMaximized)
            m_Window->Maximize();
        else
            m_Window->CenterWindow();
        m_Window->SetResizable(specification.Resizable);

        if (m_Specification.EnableImGui)
        {
            m_ImGuiLayer = new ImGuiLayer();
            PushOverlay(m_ImGuiLayer);
        }
    }

    Application::~Application()
    {
        m_Window->SetEventCallback([](Event& e) {});

        for (Layer* layer : m_LayerStack)
        {
            layer->OnDetach();
            delete layer;
        }

        Renderer::Shutdown();

        s_Instance = nullptr;
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::PopLayer(Layer* layer)
    {
        m_LayerStack.PopLayer(layer);
        layer->OnDetach();
    }

    void Application::PopOverlay(Layer* layer)
    {
        m_LayerStack.PopOverlay(layer);
        layer->OnDetach();
    }

    void Application::RenderImGui()
    {
        m_ImGuiLayer->Begin();

        for (int i = 0; i < m_LayerStack.Size(); i++)
            m_LayerStack[i]->OnImGuiRender();

        m_ImGuiLayer->End();
    }

    void Application::Run()
    {
        OnInit();

        while (m_Running)
        {
            ProcessEvents(); // Poll events when both threads are idle
            if (!m_Minimized)
            {
                //                m_Window->GetSwapChain().BeginFrame();

                Renderer::BeginFrame();
                {
                    for (Layer* layer : m_LayerStack)
                        layer->OnUpdate(m_TimeStep);
                }

                if (m_Specification.EnableImGui)
                {
                    RenderImGui();
                }
                Renderer::EndFrame();

                m_Window->SwapBuffers();
            }

            float time      = m_Window->GetTime();
            m_Frametime     = time - m_LastFrameTime;
            m_TimeStep      = glm::min<float>(m_Frametime, 0.0333f);
            m_LastFrameTime = time;
        }
        OnShutdown();
    }

    void Application::Close() { m_Running = false; }
    void Application::OnShutdown()
    {
        m_EventCallbacks.clear();
        g_ApplicationRunning = false;
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResize(e); });
        dispatcher.Dispatch<WindowMinimizeEvent>([this](WindowMinimizeEvent& e) { return OnWindowMinimize(e); });
        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return OnWindowClose(e); });

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(event);
            if (event.Handled)
                break;
        }

        if (event.Handled)
            return;

        // TODO(Peter): Should these callbacks be called BEFORE the layers recieve events?
        //				We may actually want that since most of these callbacks will be functions REQUIRED in order for
        // the game 				to work, and if a layer has already handled the event we may end up with problems
        for (auto& eventCallback : m_EventCallbacks)
        {
            eventCallback(event);

            if (event.Handled)
                break;
        }
    }

    void Application::ProcessEvents()
    {
        Input::TransitionPressedKeys();
        Input::TransitionPressedButtons();

        m_Window->ProcessEvents();

        std::scoped_lock<std::mutex> lock(m_EventQueueMutex);

        // Process custom event queue
        while (m_EventQueue.size() > 0)
        {
            auto& func = m_EventQueue.front();
            func();
            m_EventQueue.pop();
        }
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        const uint32_t width = e.GetWidth(), height = e.GetHeight();
        if (width == 0 || height == 0)
        {
            // m_Minimized = true;
            return false;
        }
        // m_Minimized = false;

        auto& window = m_Window;
        //        Renderer::Submit([&window, width, height]() mutable
        //                         {
        //                             window->GetSwapChain().OnResize(width, height);
        //                         });

        return false;
    }

    bool Application::OnWindowMinimize(WindowMinimizeEvent& e)
    {
        m_Minimized = e.IsMinimized();
        return false;
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        Close();
        return false; // give other things a chance to react to window close
    }

    const char* Application::GetConfigurationName() { return ""; }

    const char* Application::GetPlatformName() { return ""; }

} // namespace Engine