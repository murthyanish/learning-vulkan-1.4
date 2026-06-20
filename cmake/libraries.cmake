cmake_minimum_required(VERSION 4.3)

find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(X11 REQUIRED)
find_package(Threads REQUIRED)

# This takes care of vendored libraries. Also has code to use library libs
# instead if LVULKAN_<lib>_VENDORED is set to false.
add_subdirectory(${CMAKE_SOURCE_DIR}/vendored)

add_library(lvulkan_libraries INTERFACE)
target_link_libraries(lvulkan_libraries
  INTERFACE
    Vulkan::Vulkan
    ${CMAKE_DL_LIBS}
    Threads::Threads
    X11::X11
    X11::Xxf86vm
    X11::Xrandr
    X11::Xi
    lvulkan_vendored
    glm::glm
)
