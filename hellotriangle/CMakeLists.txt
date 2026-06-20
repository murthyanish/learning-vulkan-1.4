cmake_minimum_required(VERSION 4.3)

project(lvulkan-hellotriangle)

add_executable(lvulkan_hellotriangle)
target_compile_features(lvulkan_hellotriangle PRIVATE cxx_std_17)

target_sources(lvulkan_hellotriangle
  PRIVATE
    lvulkan_hellotriangle.cpp
)

# target link libraries
target_link_libraries(lvulkan_hellotriangle
  PRIVATE
    lvulkan_libraries
)
