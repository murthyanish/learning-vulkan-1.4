cmake_minimum_required(VERSION 4.3)

project(lvulkan-lesson-0)

add_executable(lvulkan_0)
target_compile_features(lvulkan_0 PRIVATE cxx_std_17)

target_sources(lvulkan_0
  PRIVATE
    lvulkan_0.cpp
)

# target link libraries
target_link_libraries(lvulkan_0
  PRIVATE
    vendor
    lvulkan_libraries
)
