

#include "Application/Application.h"
#include "Core/Core.h"

Application::Application()
{
    window = new WaylandWindow();
} // Application c-tor

Application::~Application()
{
    delete window;
} // ~Application d-tor

void Application::Run()
{
    AND_DEBUG("This is a debug build!");
    window->Initialize();
    window->Run();
}

    
