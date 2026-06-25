cmake_minimum_required(VERSION 4.3)

### Setting up module support
# I'm using 23 instead of 20 because module std is only supported in 23.
#
# I might just remove this entirely and focus only on 23 later. I don't want
# to spend more time dealing with setup and compatibility issues.

# Add option to enable/disable C++ 23 module
option(ENABLE_CPP23_MODULE "Enable C++ 23 module support for Vulkan" ON)

# Enable C++ module dependency scanning only if C++ 20 module is enabled
if(ENABLE_CPP23_MODULE)
  set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
  # Explicitly tell CMake this target consumes standard library modules
  set(CMAKE_CXX_MODULE_STD ON)
  # Tell CMake to scan source files for module dependency mapping
  set(CMAKE_CXX_SCAN_FOR_MODULES ON)
endif()

project(learning-vulkan-1.4)

### Set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory
# automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

### Importing libraries.
# I'm placing all the libraries in this cmake to make things easier. I'm
# exporting the target lvulkan_libraries so I can quickly use them in the
# project.
include(CMake/libraries.cmake)
# include(CMake/project_utilities.cmake)

### A test cpp built from the old vulkan-tutorial code.
option(LVULKAN_L0 "Build lesson 0" OFF)
if(LVULKAN_L0)
  add_subdirectory(lesson-0)
endif()

option(LVULKAN_HELLOTRIANGLE "Build HelloTriangleApplication" ON)
if(LVULKAN_HELLOTRIANGLE)
  add_subdirectory(hellotriangle)
endif()
