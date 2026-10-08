cmake_minimum_required(VERSION 4.3)

project(lvulkan_hellotriangle)

add_executable(lvulkan_hellotriangle)

set_target_properties(lvulkan_hellotriangle PROPERTIES CXX_MODULE_STD ON)

target_compile_features(lvulkan_hellotriangle PRIVATE cxx_std_23)

target_sources(lvulkan_hellotriangle
  PRIVATE
    lvulkan_hellotriangle.cpp
    lsdl_vk_instance-instance.cpp
    lsdl_vk_instance-debug_messenger.cpp
  PUBLIC
    FILE_SET CXX_MODULES FILES
      lsdl_resources.cppm
      lsdl_vk_instance.cppm
      lsdl_vk_instance-validation.cppm
      lsdl_vk_instance-instance.cppm
      lsdl_vk_instance-debug_messenger.cppm
      # lvk_render_device.cppm
)

# target link libraries
target_link_libraries(lvulkan_hellotriangle
  PRIVATE
    lvulkan_libraries
)

# target_include_directories (lvulkan_hellotriangle PRIVATE ${STB_INCLUDEDIR})
# target_compile_definitions(${CHAPTER_NAME} PRIVATE USE_CPP23_MODULES=1)
target_compile_definitions(lvulkan_hellotriangle
  PRIVATE
    "VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS"
)
