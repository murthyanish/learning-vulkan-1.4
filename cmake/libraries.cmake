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
)
