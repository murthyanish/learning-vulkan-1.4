cmake_minimum_required(VERSION 4.3)

### Enabling cxx20 module support with std module.
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
# Explicitly tell CMake this target consumes standard library modules
set(CMAKE_CXX_MODULE_STD ON)
# Tell CMake to scan source files for module dependency mapping
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(learning-vulkan-1.4 CXX)

### Setting up a global cxx version.
# (Probably not the best idea, but I want to move this along.)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

### Set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory
# automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

### Importing libraries.
# I'm placing all the libraries in this cmake to make things easier. I'm
# exporting the target lvulkan_libraries so I can quickly use them in the
# project.
include(CMake/libraries.cmake)

### A test cpp built from the old vulkan-tutorial code.
option(LVULKAN_L0 "Build lesson 0" OFF)
if(LVULKAN_L0)
  add_subdirectory(lesson-0)
endif()

option(LVULKAN_HELLOTRIANGLE "Build HelloTriangleApplication" ON)
if(LVULKAN_HELLOTRIANGLE)
  add_subdirectory(hellotriangle)
endif()
