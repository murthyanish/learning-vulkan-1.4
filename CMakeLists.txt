cmake_minimum_required(VERSION 4.3)

project(learning-vulkan-1.4)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Option to switch between a system sdl library and a vendored SDL library
option(LVULKAN_SDL_VENDORED "Use vendored SDL libraries" OFF)

# This takes care of vendored libraries. Also has code to use library libs
# instead if LVULKAN_<lib>_VENDORED is set to false.
add_subdirectory(vendored)
include(cmake/libraries.cmake)

find_package(Vulkan REQUIRED)
find_package(X11 REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(lesson-0)
