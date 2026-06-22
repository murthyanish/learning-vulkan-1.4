cmake_minimum_required(VERSION 4.3)

include(FetchContent)

# Option to switch between a system sdl library and a vendored SDL library
option(LVULKAN_SDL_VENDORED "Use vendored SDL libraries" OFF)

set(SDL_TEST OFF CACHE BOOL "")
set(SDL_EXAMPLES OFF CACHE BOOL "")

if(LVULKAN_SDL_VENDORED)
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        main # Or use a specific tag like release-3.2.0
    )

    FetchContent_MakeAvailable(SDL3)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

add_library(lvulkan_vendored INTERFACE)
target_link_libraries(lvulkan_vendored INTERFACE SDL3::SDL3)
