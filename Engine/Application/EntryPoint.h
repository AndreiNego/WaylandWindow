#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

extern Application* CreateApplication();

int main() //(int argc, char** argv)
{
    auto app = CreateApplication();
    app->Run();
    delete app;
}

#endif //ENTRY_POINT_H
