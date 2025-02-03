project(WaylandWindow VERSION 0.1 LANGUAGES CXX)

# Automatically find all .cpp source files
file(GLOB_RECURSE APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Create executable
add_executable(WaylandWindow ${APP_SOURCES})

# Include headers and link the shared library
target_include_directories(WaylandWindow PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${CMAKE_BINARY_DIR})
target_link_libraries(WaylandWindow PRIVATE Engine)

# Define preprocessor flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(WaylandWindow PRIVATE _DEBUG)
    target_compile_options(WaylandWindow PRIVATE -Wall -Wextra -Wpedantic -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(WaylandWindow PRIVATE _RELEASE)
    target_compile_options(WaylandWindow PRIVATE -O3 -flto)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Debug" FORCE)
endif()
