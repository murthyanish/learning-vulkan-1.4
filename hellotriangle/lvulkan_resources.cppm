module;

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_vulkan.h>
#include <stdexcept>

// Including `vk_platform.h` is mandatory because platform specific compiler
// macros exist which cannot be replicated with any native C++ functionality.
#include <vulkan/vk_platform.h>

export module lvulkan_resources;

import vulkan;
import std;
import lsdl_resources;

// Validation Layers
#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
constexpr bool enableValidationCallbacks = false;
#else
constexpr bool enableValidationLayers = true;
#endif

const std::vector<char const *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

export struct LSDLVulkanInstance {
public:
  explicit LSDLVulkanInstance(LSDLSubsystem &sdlVideo, // To enforce dependency
                              LSDLWindow &sdlWindow,   // To enforce dependency
                              std::string_view application_name,
                              bool enable_validation_layers = false)
      : context() {
    vk::ApplicationInfo appInfo{.pApplicationName = application_name.data(),
                                .applicationVersion = vk::makeVersion(1, 0, 0),
                                .pEngineName = "No Engine",
                                .engineVersion = vk::makeVersion(1, 0, 0),
                                .apiVersion = vk::ApiVersion14};

    // Get the required extensions from SDL and set count to the number of
    // extensions
    auto requiredExtensions = getRequiredInstanceExtensions();

    // Get the required layers
    std::vector<char const *> requiredLayers;
    if (enableValidationLayers) {
      requiredLayers.assign(validationLayers.begin(), validationLayers.end());
    }

    // Check if the required SDL extensions are supported by the Vulkan
    // implementation.
    auto extensionProperties = context.enumerateInstanceExtensionProperties();
    auto unsupportedPropertyIt = std::ranges::find_if(
        requiredExtensions,
        [&extensionProperties](auto const &requiredExtension) {
          return std::ranges::none_of(
              extensionProperties,
              [requiredExtension](auto const &extensionProperty) {
                return strcmp(extensionProperty.extensionName,
                              requiredExtension) == 0;
              });
        });
    if (unsupportedPropertyIt != requiredExtensions.end()) {
      throw std::runtime_error("Required extension not supported: " +
                               std::string(*unsupportedPropertyIt));
    }

    // Check if the required layers are supported by the Vulkan implementation.
    auto layerProperties = context.enumerateInstanceLayerProperties();
    auto unsupportedLayerIt = std::ranges::find_if(
        requiredLayers, [&layerProperties](auto const &requiredLayer) {
          return std::ranges::none_of(
              layerProperties, [requiredLayer](auto const &layerProperty) {
                return strcmp(layerProperty.layerName, requiredLayer) == 0;
              });
        });
    if (unsupportedLayerIt != requiredLayers.end()) {
      throw std::runtime_error("Required layer not supported: " +
                               std::string(*unsupportedLayerIt));
    }

    vk::InstanceCreateInfo createInfo{
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
        .ppEnabledLayerNames = requiredLayers.data(),
        .enabledExtensionCount =
            static_cast<uint32_t>(requiredExtensions.size()),
        .ppEnabledExtensionNames = requiredExtensions.data()};

    instance = vk::raii::Instance(context, createInfo);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Vulkan Instance created!");
  }

  const vk::raii::Instance &get() { return instance; }
  const vk::raii::Context &getContext() { return context; }

  ~LSDLVulkanInstance() = default;

  LSDLVulkanInstance(const LSDLVulkanInstance &) = delete;
  LSDLVulkanInstance &operator=(const LSDLVulkanInstance &) = delete;

private:
  vk::raii::Context context;
  vk::raii::Instance instance = nullptr;

  std::vector<const char *> getRequiredInstanceExtensions() {
    uint32_t extensionCount = 0;
    const char *const *sdlExtensions =
        SDL_Vulkan_GetInstanceExtensions(&extensionCount);

    if (sdlExtensions == NULL) {
      throw std::runtime_error(
          std::string("SDL_Vulkan_GetInstanceExtensions failed: ") +
          SDL_GetError());
    }

    std::vector extensions(sdlExtensions, sdlExtensions + extensionCount);
    if (enableValidationLayers) {
      extensions.push_back(vk::EXTDebugUtilsExtensionName);
    }

    return extensions;
  }
};

// I have no idea why my editor wants to format this function weirdly. I added
// the below to force it to stop messing with my formatting.
// clang-format off
static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(
    vk::DebugUtilsMessageSeverityFlagBitsEXT       severity,
    vk::DebugUtilsMessageTypeFlagsEXT              type,
    const vk::DebugUtilsMessengerCallbackDataEXT * pCallbackData,
    void *                                         pUserData) {
  // clang-format on

  switch (severity) {
  case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
  case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
    SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "validation layer: type %s msg: %s",
                vk::to_string(type).c_str(), pCallbackData->pMessage);
  case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
    SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "validation layer: type %s msg: %s",
                vk::to_string(type).c_str(), pCallbackData->pMessage);

  case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "validation layer: type %s msg: %s",
                 vk::to_string(type).c_str(), pCallbackData->pMessage);
  }

  return vk::False;
}

export struct LVulkanDebugMessenger {
public:
  explicit LVulkanDebugMessenger(LSDLVulkanInstance &instance) {
    if (!enableValidationLayers)
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

  ~LVulkanDebugMessenger() = default;

  LVulkanDebugMessenger(const LVulkanDebugMessenger &) = delete;
  LVulkanDebugMessenger &operator=(const LVulkanDebugMessenger &) = delete;

private:
  vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;
};
