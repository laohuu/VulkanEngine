#ifndef ENGINE_ENTRYPOINT_H
#define ENGINE_ENTRYPOINT_H

#include "Application.h"

extern Engine::Application* Engine::CreateApplication(int argc, char** argv);
bool                        g_ApplicationRunning = true;

int main(int argc, char** argv)
{
    while (g_ApplicationRunning)
    {
        auto app = Engine::CreateApplication(argc, argv);
        app->Run();
        delete app;
    }
    return 0;
}

#endif // ENGINE_ENTRYPOINT_H
