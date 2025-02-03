

#include "Linux/WaylandWindow.h"
#include "Core/Core.h"
#include "Linux/WaylandGenFiles/xdg-decoration-unstable-v1-client-protocol.h"
#include "Linux/WaylandGenFiles/xdg-shell-client-protocol.h"

#include <cstdint>
#include <cstring>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>



#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"



const wl_callback_listener callbackListener = {
    &WaylandWindow::NewFrame };
const xdg_wm_base_listener shellListener = {
    &WaylandWindow::ShellPing 

};

 const xdg_toplevel_listener TopListener = {
        &WaylandWindow::TopConfigure,
        &WaylandWindow::TopClose,
        &WaylandWindow::ConfigureTopBounds,
        &WaylandWindow::TopWMCapabilities
    };
const wl_seat_listener seatListener = {
    &WaylandWindow::SeatCapabilities,
    &WaylandWindow::SeatName
};
WaylandWindow::WaylandWindow()
{
    
}

WaylandWindow::~WaylandWindow()
{
    wl_seat_release(seat);
    if (buffer != nullptr)
    {
        wl_buffer_destroy(buffer);
    }
    xdg_toplevel_destroy(topLevel);
    xdg_surface_destroy(xdgSurface);
    wl_display_disconnect(display);
}

void WaylandWindow::Initialize()
{
    display = wl_display_connect(nullptr);
    registry = wl_display_get_registry(display);
    
    static const wl_registry_listener registryListener = {
        &WaylandWindow::RegistryGlobal,
        &WaylandWindow::RegistryGlobalRemove
    };

    wl_registry_add_listener(registry, &registryListener, this);
    wl_display_roundtrip(display);

    surface = wl_compositor_create_surface(compositor);
    
    if(!surface)
    {
        AND_FATAL("Failed to create surface!");
    }

    frameCallback = wl_surface_frame(surface);

    wl_callback_add_listener(frameCallback, &callbackListener, this);

    xdgSurface = xdg_wm_base_get_xdg_surface(shell, surface);

    if (!xdgSurface)
    {
        AND_FATAL("Failed to create xdgSurface");
    }
    else {
        AND_DEBUG("Created xdgSurface");
    }
    static const xdg_surface_listener XDGSurfaceListener = {
        &WaylandWindow::XDGSurfaceConfig
    };
    xdg_surface_add_listener(xdgSurface, &XDGSurfaceListener, this);
    topLevel = xdg_surface_get_toplevel(xdgSurface);
    
    if (!topLevel)
    {
        AND_FATAL("Failed to create XDG Top Level");
    }
    else {
        AND_DEBUG("Created top level");
    }



    xdg_toplevel_add_listener(topLevel, &TopListener, this);
    xdg_toplevel_set_title(topLevel, windowName);
    wl_surface_commit(surface);
    wl_display_roundtrip(display);
    RequestDecorations(decorationManager, topLevel);
}

void WaylandWindow::Run()
{   
    if (!display)
    {
    AND_FATAL("No Display!");
    }
    else {
        AND_DEBUG("Display: ");
    }
    while(isRunning) {
        int ret = wl_display_dispatch(display);
        if (ret == -1) {
            break;
        }
    }
}

void WaylandWindow::RegistryGlobal(void* data, wl_registry* registry, uint32_t name, const char* iface, uint32_t version)
{
    auto window = static_cast<WaylandWindow*>(data);
    if (std::string(iface) == wl_compositor_interface.name) {
        window->compositor = static_cast<wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, version));
        AND_DEBUG("Compositor bound: {}", name);
    }
    else if (std::string(iface) == wl_shm_interface.name) {
        window->shm = static_cast<wl_shm*>(wl_registry_bind(registry, name, &wl_shm_interface, version));
        AND_DEBUG("Shm bound: {}", name);
    }
    else if (std::string(iface) == xdg_wm_base_interface.name) {
        window->shell = static_cast<xdg_wm_base*>(wl_registry_bind(registry, name, &xdg_wm_base_interface, version));
        xdg_wm_base_add_listener(window->shell, &shellListener, nullptr);
        AND_DEBUG("Wm Base bound: {}", name);
    }
    else if (std::string(iface) == wl_seat_interface.name) {
        window->seat = static_cast<wl_seat*>(wl_registry_bind(registry, name, &wl_seat_interface, version));
        wl_seat_add_listener(window->seat, &seatListener, nullptr);
        AND_DEBUG("Seat bound: {}", name);
    }
    else if (std::string(iface) == zxdg_decoration_manager_v1_interface.name) {
        window->decorationManager = static_cast<zxdg_decoration_manager_v1*>(wl_registry_bind(registry, name, &zxdg_decoration_manager_v1_interface, version));
        AND_DEBUG("Decoration manager bound: {}", name);
    }
}

void WaylandWindow::RegistryGlobalRemove(void* data, wl_registry* registry, uint32_t name)
{
}

void WaylandWindow::TopConfigure(void* data, xdg_toplevel* topLvl, int32_t newWidth, int32_t newHeight, wl_array* states)
{
    auto window = static_cast<WaylandWindow*>(data);
    if(newWidth > 0 || newHeight > 0)
    {
              window->HandleTopConfigure(newWidth, newHeight);
    }
    bool handled = false;

    uint32_t* state;
#define WL_ARRAY_FOR_EACH(pos, array, type) for (pos = (type)(array)->data; (const char*) pos < ((const char*) (array)-> data + (array)-> size); (pos)++)
    WL_ARRAY_FOR_EACH(state, states, uint32_t*) {
    if(handled) break;
    if (*state == XDG_TOPLEVEL_STATE_MAXIMIZED && !handled) {
        if (!window->maximized)
        {
            xdg_toplevel_set_maximized(window->topLevel);
            window->maximized = true;
            handled = true;
                    }
        else
        {
            xdg_toplevel_unset_maximized(window->topLevel);
            window->maximized = false;
            handled = true;
                  
        }
    }
    else if (*state == XDG_TOPLEVEL_STATE_FULLSCREEN && window->fullscreen == false) {
        xdg_toplevel_set_fullscreen(window->topLevel, nullptr);
        window->fullscreen = true;
        break;
    }
    else if (*state == XDG_TOPLEVEL_STATE_FULLSCREEN && window->fullscreen == true)
    {
        xdg_toplevel_unset_fullscreen(window->topLevel);
        window->fullscreen = false;
        break;
    }
     
    }
}

void WaylandWindow::HandleTopConfigure(int32_t newWidth, int32_t newHeight)
{
        munmap(mappedMemory, newWidth * newHeight * 4);
        width = newWidth;
        height = newHeight;
        ResizeSharedMem();
}

void WaylandWindow::TopClose(void* data, xdg_toplevel* topLvl)
{
    AND_DEBUG("Window should close. ");
    static_cast<WaylandWindow*>(data)->isRunning = false;
}

void WaylandWindow::TopWMCapabilities(void *data, xdg_toplevel *topLvl, wl_array *array)
{

}

void WaylandWindow::ConfigureTopBounds(void *data, xdg_toplevel *topLvl, int32_t width, int32_t height)
{

}
void WaylandWindow::ShellPing(void* data, xdg_wm_base* shell, uint32_t serial)
{
    xdg_wm_base_pong(shell, serial);
}

void WaylandWindow::SeatCapabilities(void* data, wl_seat* seat, uint32_t capabilities)
{

}

void WaylandWindow::SeatName(void* data, wl_seat* seat, const char* name)
{

}

void WaylandWindow::XDGSurfaceConfig(void* data, xdg_surface* xdgSurface, uint32_t serial)
{   
    auto window = static_cast<WaylandWindow*>(data);
    if (!window)
    {
        AND_FATAL("Data pointer is null");
        return;
    }
    xdg_surface_ack_configure(xdgSurface, serial);

    window->ResizeSharedMem();

    window->Draw();
}

int32_t WaylandWindow::AllocateSharedMem(size_t size)
{
    std::string shFileName = "/wayland-shm-" + std::to_string(getpid());

    int fileDescriptor = shm_open(shFileName.c_str(), O_RDWR | O_CREAT | O_EXCL, 0600);

    if(fileDescriptor < 0) 
    {
        close(fileDescriptor);
        AND_FATAL("Failed to allocate shared memory, FileDescriptor: {}", fileDescriptor);
    }

    shm_unlink(shFileName.c_str());

    if (ftruncate(fileDescriptor, width * height * 4) < 0)
    {
        close(fileDescriptor);
        AND_FATAL("Failed to truncate memory.");
    }

    return fileDescriptor;
}

void WaylandWindow::ResizeSharedMem()
{   
    int32_t fileDescriptor = AllocateSharedMem(width * height * 4);

    mappedMemory = mmap(nullptr, width * height * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);

    if (mappedMemory == MAP_FAILED)
    {
        close(fileDescriptor);
        AND_FATAL("Failed to map memory!");
    }

    wl_shm_pool* pool = wl_shm_create_pool(shm, fileDescriptor, width * height * 4);

    buffer = wl_shm_pool_create_buffer(pool, 0, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);

    wl_shm_pool_destroy(pool);

    close(fileDescriptor);
}

void WaylandWindow::Draw()
{
    uint32_t color = 0xFF0000FF;
    std::fill_n(static_cast<uint32_t*>(mappedMemory), width * height, color);
    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_damage(surface, 0, 0, width, height);
    wl_surface_commit(surface);
}

void WaylandWindow::NewFrame(void* data, wl_callback* callback, uint32_t callbackData)
{
    auto window = static_cast<WaylandWindow*>(data);
    if (!window)
    {
        AND_FATAL("Data pointer is null!");
        return;
    }
   
    window->HandleNewFrame(callback);
}
void WaylandWindow::HandleNewFrame(wl_callback* callback)
{
    wl_callback_destroy(callback);
    frameCallback = wl_surface_frame(surface);
    wl_callback_add_listener(frameCallback, &callbackListener, this);
    counter++;
    Draw();
}

void WaylandWindow::RequestDecorations(zxdg_decoration_manager_v1* decorationManager, xdg_toplevel* topLvl)
{
    zxdg_toplevel_decoration_v1* decoration = zxdg_decoration_manager_v1_get_toplevel_decoration(decorationManager, topLevel);
    zxdg_toplevel_decoration_v1_set_mode(decoration, ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
}

#pragma GCC diagnostic pop
