#include "Application.h"

#include "Input.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

namespace Engine
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification& specification) : m_Specification(specification)
    {
        s_Instance = this;

        //        WindowSpecification windowSpec;
        //        windowSpec.Title      = specification.Name;
        //        windowSpec.Width      = specification.Width;
        //        windowSpec.Height     = specification.Height;
        //        windowSpec.Decorated  = false;
        //        windowSpec.Fullscreen = false;
        //        windowSpec.VSync      = true;
        //        m_Window              = std::unique_ptr<Window>(Window::Create(windowSpec));
        //        m_Window->Init();
        //        m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

        // Init renderer and execute command queue to compile all shaders
        //        Renderer::Init();

        //        if (false)
        //            m_Window->Maximize();
        //        else
        //            m_Window->CenterWindow();

        Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        Shutdown();
        s_Instance = nullptr;
    }

    void Application::Init()
    {
        // Setup GLFW window
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
        {
            std::cerr << "Could not initalize GLFW!\n";
            return;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_WindowHandle = glfwCreateWindow(
            m_Specification.Width, m_Specification.Height, m_Specification.Name.c_str(), nullptr, nullptr);
    }

    void Application::Shutdown()
    {
        for (auto& layer : m_LayerStack)
            layer->OnDetach();

        m_LayerStack.Clear();

        glfwDestroyWindow(m_WindowHandle);
        glfwTerminate();
    }

    void Application::Close() { m_Running = false; }

    float Application::GetTime() { return (float)glfwGetTime(); }

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

    void Application::SubmitToMainThread(const std::function<void()>& function)
    {
        std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

        m_MainThreadQueue.emplace_back(function);
    }

    void Application::Run()
    {
        m_Running = true;

        while (!glfwWindowShouldClose(m_WindowHandle) && m_Running)
        {
            float time      = GetTime();
            m_Frametime     = time - m_LastFrameTime;
            m_TimeStep      = glm::min<float>(m_Frametime, 0.0333f);
            m_LastFrameTime = time;

            ExecuteMainThreadQueue();

            if (!m_Minimized)
            {
                {
                    for (Layer* layer : m_LayerStack)
                        layer->OnUpdate(m_TimeStep);
                }

                m_ImGuiLayer->Begin();
                {
                    for (Layer* layer : m_LayerStack)
                        layer->OnImGuiRender();
                }
                m_ImGuiLayer->End();
            }
        }
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

    void Application::ExecuteMainThreadQueue()
    {
        std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

        for (auto& func : m_MainThreadQueue)
            func();

        m_MainThreadQueue.clear();
    }
} // namespace Engine