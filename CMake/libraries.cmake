cmake_minimum_required(VERSION 4.3)

include(FetchContent)

find_package (glm REQUIRED)
find_package (VulkanHpp 1.4.335 REQUIRED)      # Require Vulkan SDK version 1.4.335 or higher
find_package (tinyobjloader REQUIRED)
find_package (tinygltf REQUIRED)
find_package (KTX REQUIRED)

find_package(stb REQUIRED)
set(STB_INCLUDEDIR ${stb_INCLUDE_DIRS})

# This takes care of vendored libraries. Also has code to use library libs
# instead if LVULKAN_<lib>_VENDORED is set to false.
add_subdirectory(${CMAKE_SOURCE_DIR}/vendored)

add_library(lvulkan_libraries INTERFACE)
target_link_libraries(lvulkan_libraries
  INTERFACE
    VulkanHpp::VulkanHpp
    lvulkan_vendored
    glm::glm
)
