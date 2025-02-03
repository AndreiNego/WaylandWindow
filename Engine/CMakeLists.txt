project(Engine VERSION 0.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")

find_package(PkgConfig  REQUIRED)
pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
# Automatically find all .cpp source files
file(GLOB_RECURSE LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c*")
set(GENERATEDWL Linux/WaylandGenFiles/xdg-shell-protocol.c Linux/WaylandGenFiles/xdg-decoration-unstable-v1-client-protocol.c)
list(APPEND LIB_SOURCES Linux/WaylandGenFiles/xdg-shell-protocol.c Linux/WaylandGenFiles/xdg-decoration-unstable-v1-client-protocol.c)
# Create a shared library
add_library(Engine SHARED ${LIB_SOURCES})

# Include headers
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Engine PUBLIC ${WAYLAND_CLIENT_LIBRARIES})

# Define preprocessor flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Engine PRIVATE _DEBUG)
    target_compile_options(Engine PRIVATE -Wall -Wextra -Wpedantic -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(Engine PRIVATE _RELEASE -DNDEBUG)
    target_compile_options(Engine PRIVATE -O3 -flto)
endif()
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()
