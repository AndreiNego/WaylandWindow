
#include "Linux/WaylandWindow.h"

#ifndef APPLICATION_H
#define APPLICATION_H


class Application
{

public:
    Application();
    virtual ~Application();

    void Run();
private:
    WaylandWindow* window;
};

    Application* CreateApplication();


#endif //APPLICATION_H
