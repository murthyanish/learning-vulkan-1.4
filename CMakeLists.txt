cmake_minimum_required(VERSION 4.3)

project(learning-vulkan-1.4)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

include(cmake/libraries.cmake)


option(lvulkan_l0 "Build lesson 0" OFF)
if(lvulkan_l0)
  add_subdirectory(lesson-0)
endif()

option(lvulkan_hellotriangle "Build HelloTriangleApplication" ON)
add_subdirectory(hellotriangle)
