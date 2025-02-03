

#ifndef WAYLAND_WINDOW_H
#define WAYLAND_WINDOW_H

#include <cstdint>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>


// include generated header file dependencies
extern "C" {
#include "Linux/WaylandGenFiles/xdg-decoration-unstable-v1-client-protocol.h"
#include "Linux/WaylandGenFiles/xdg-shell-client-protocol.h"
}


class WaylandWindow
{
public:
    WaylandWindow();
    ~WaylandWindow();

    void Run();

    void Initialize();

    static void RegistryGlobal(void* data, wl_registry* registry, uint32_t name, const char* iface, uint32_t version);
    static void RegistryGlobalRemove(void* data, wl_registry* registry, uint32_t name);
    static void TopConfigure(void* data, xdg_toplevel* topLevel, int32_t new_width, int32_t new_height, wl_array* states);
    static void TopClose(void* data, xdg_toplevel* topLvl);
    void HandleTopConfigure(int32_t newWidth, int32_t newHeight);
    static void ConfigureTopBounds(void*data, xdg_toplevel* topLvl, int32_t width, int32_t height);
    static void TopWMCapabilities(void* data, xdg_toplevel* topLvl, wl_array* array);
    static void ShellPing(void* data, xdg_wm_base* shell, uint32_t serial);
    static void SeatCapabilities(void* data, wl_seat* seat, uint32_t capabilities);
    static void SeatName(void* data, wl_seat* seat, const char* name);
    static void XDGSurfaceConfig(void* data, xdg_surface* xdgSurface, uint32_t serial);

    int32_t AllocateSharedMem(size_t size);
    void ResizeSharedMem();
    void Draw();
    static void NewFrame(void* data, wl_callback* callback, uint32_t callbackData);
    void HandleNewFrame(wl_callback* callback);
    void RequestDecorations(zxdg_decoration_manager_v1* decorationManager, xdg_toplevel* topLevel);

    wl_display* display = nullptr;
    wl_registry* registry = nullptr;
    wl_compositor* compositor = nullptr;
    xdg_wm_base* shell = nullptr;
    xdg_toplevel* topLevel = nullptr;
    wl_seat* seat = nullptr;
    zxdg_decoration_manager_v1* decorationManager = nullptr;
    wl_shm* shm = nullptr;

    void* mappedMemory;

    wl_surface* surface = nullptr;
    wl_callback* frameCallback = nullptr;
    wl_buffer* buffer = nullptr;
    xdg_surface* xdgSurface = nullptr;

    int32_t width {1600};
    int32_t height {900};
    int8_t counter {0};
    bool isRunning {true};


    bool maximized {false};
    bool fullscreen {false};
    const char* windowName {"Test"};
};


#endif //WAYLAND_WINDOW_H
