module;

#include <SDL3/SDL_log.h>

// Including `vk_platform.h` is mandatory because platform specific compiler
// macros exist which cannot be replicated with any native C++ functionality.
#include <vulkan/vk_platform.h>

export module lsdl_vk_instance:debug_messenger;

import vulkan;
import std;
import :instance;
import :validation;

namespace LVulkan {

// I have no idea why my editor wants to format this function weirdly. I added
// the below to force it to stop messing with my formatting.
// clang-format off
static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(
    vk::DebugUtilsMessageSeverityFlagBitsEXT       severity,
    vk::DebugUtilsMessageTypeFlagsEXT              type,
    const vk::DebugUtilsMessengerCallbackDataEXT * pCallbackData,
    void *                                         pUserData) {
  // clang-format on

  // The Khronos Vulkan tutorial doesn't use `vk::to_string` for some reason,
  // I only found out this existed later, and it seems super useful, so I used
  // this instead of the `std::to_string` call that the tutorial uses.
  switch (severity) {
  case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
  case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
    SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "validation layer: type %s msg: %s",
                vk::to_string(type).c_str(), pCallbackData->pMessage);
    break;
  case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
    SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "validation layer: type %s msg: %s",
                vk::to_string(type).c_str(), pCallbackData->pMessage);
    break;
  case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "validation layer: type %s msg: %s",
                 vk::to_string(type).c_str(), pCallbackData->pMessage);
    break;
  }

  return vk::False;
}

export struct LVkDebugMessenger {
public:
  explicit LVkDebugMessenger(LSDLVkInstance &instance) {
    if (!enableValidationLayers || !enableValidationCallbacks)
      return;

    // I'm aware that I'm only enabling warnings and errors here, but have a
    // switch for all of them in the callback. Probably something to fix later.
    // TODO: Maybe change this?
    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
    vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{
        .messageSeverity = severityFlags,
        .messageType = messageTypeFlags,
        .pfnUserCallback = &debugCallback};
    debugMessenger = instance.get().createDebugUtilsMessengerEXT(
        debugUtilsMessengerCreateInfoEXT);
  };

  ~LVkDebugMessenger() = default;

  LVkDebugMessenger(const LVkDebugMessenger &) = delete;
  LVkDebugMessenger &operator=(const LVkDebugMessenger &) = delete;

private:
  vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;
};

} // namespace LVulkan
