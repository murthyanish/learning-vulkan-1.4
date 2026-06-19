set(SDL_TEST OFF)

if(LVULKAN_SDL_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(SDL EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE SDL3::SDL3)
