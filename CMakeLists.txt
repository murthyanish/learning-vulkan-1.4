cmake_minimum_required(VERSION 4.3)

project(learning-vulkan-1.4)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored)

find_package(Vulkan REQUIRED)
find_package(X11 REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(lesson-0)
