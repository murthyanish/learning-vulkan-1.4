# TODO List

## Engine

[ ] Uniform logging

## CMake

[ ] CPM to load libraries

## Useful libraries

[ ] ImGui (cgltf is a light version but not as good for engines)
[ ] volk - Vulkan meta-loader. Need to do some research on what this is and if
  it's even needed with RAII.
[ ] VulkanMemoryAllocator

## Vulkan

[x] Implement VK_EXT_debug_utils: By default, Vulkan doesn't give much debug
  info. This can be used to add callbacks to handle errors.
  (<https://docs.vulkan.org/refpages/latest/refpages/source/VK_EXT_debug_report.html>)

## Others

[ ] Check if I can contribute to the Vulkan Tutorial. There's issues related to
  modules that I solved, which I shouldn't have had to solve.
