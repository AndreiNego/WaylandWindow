
#include "../Engine/Engine.h"

class Test : public Application
{
public:
    Test()
    {
        AND_FATAL("This is a message from the executable! {}", 1000);
        AND_ERROR("Error test from the executable");
        AND_WARNING("This is a warning message ");
        AND_INFO("This is an info message {}", 0.0F);
    }
    ~Test()
    {

    }
};

    Application* CreateApplication()
{
    return new Test();
}
