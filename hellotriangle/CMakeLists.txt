cmake_minimum_required(VERSION 4.3)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
# Explicitly tell CMake this target consumes standard library modules
set(CMAKE_CXX_MODULE_STD ON)
# Tell CMake to scan source files for module dependency mapping
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

project(lvulkan_hellotriangle)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(lvulkan_hellotriangle)

target_compile_features(lvulkan_hellotriangle PRIVATE cxx_std_23)

target_sources(lvulkan_hellotriangle
  PRIVATE
    lvulkan_hellotriangle.cpp
)

# target link libraries
target_link_libraries(lvulkan_hellotriangle
  PRIVATE
    lvulkan_libraries
)
target_include_directories (lvulkan_hellotriangle PRIVATE ${STB_INCLUDEDIR})
# target_compile_definitions(${CHAPTER_NAME} PRIVATE USE_CPP23_MODULES=1)
target_compile_definitions(lvulkan_hellotriangle PRIVATE "VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS" )
