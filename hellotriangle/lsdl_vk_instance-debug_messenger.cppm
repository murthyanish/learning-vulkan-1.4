module;

// Including `vk_platform.h` is mandatory because platform specific compiler
// macros exist which cannot be replicated with any native C++ functionality.
#include <vulkan/vk_platform.h>

export module lsdl_vk_instance:debug_messenger;

import vulkan;
import std;
import :instance;

namespace LVulkan {

// I have no idea why my editor wants to format this function weirdly. I added
// the below to force it to stop messing with my formatting.
// clang-format off
static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(
    vk::DebugUtilsMessageSeverityFlagBitsEXT       severity,
    vk::DebugUtilsMessageTypeFlagsEXT              type,
    const vk::DebugUtilsMessengerCallbackDataEXT * pCallbackData,
    void *                                         pUserData);

export struct LVkDebugMessenger {
public:
  explicit LVkDebugMessenger(LSDLVkInstance &instance);

  ~LVkDebugMessenger() = default;

  LVkDebugMessenger(const LVkDebugMessenger &) = delete;
  LVkDebugMessenger &operator=(const LVkDebugMessenger &) = delete;

private:
  vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;
};

} // namespace LVulkan
