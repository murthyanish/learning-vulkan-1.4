module;

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_vulkan.h>

module lsdl_vk_instance;

import vulkan;
import std;
import :validation;

namespace LVulkan {

LSDLVkInstance::LSDLVkInstance(LSDLSubsystem &sdlVideo, // To enforce dependency
                               LSDLWindow &sdlWindow,   // To enforce dependency
                               std::string_view application_name,
                               bool enable_validation_layers)
    : context(),
      instance(context,
               makeInstanceCreateInfo(
                   context, makeApplicationInfo(application_name),
                   getRequiredInstanceExtensions(), getRequiredLayers())) {
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Vulkan Instance created!");
}

std::vector<vk::raii::PhysicalDevice>
LSDLVkInstance::getPhysicalDevices() const {
  return instance.enumeratePhysicalDevices();
}

std::vector<const char *> LSDLVkInstance::getRequiredInstanceExtensions() {
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

/// @brief Get the requires layers for the engine.
///
/// @return Vector of layer names.
std::vector<const char *> LSDLVkInstance::getRequiredLayers() {
  return enableValidationLayers
             ? std::vector<const char *>(validationLayers.begin(),
                                         validationLayers.end())
             : std::vector<const char *>{};
}

/// @brief Make an ApplicationInfo struct.
///
/// @param application_name Application/Game name
/// @return ApplicationInfo struct.
vk::ApplicationInfo
LSDLVkInstance::makeApplicationInfo(std::string_view application_name) {
  return vk::ApplicationInfo{.pApplicationName = application_name.data(),
                             .applicationVersion = vk::makeVersion(1, 0, 0),
                             .pEngineName = "No Engine",
                             .engineVersion = vk::makeVersion(1, 0, 0),
                             .apiVersion = vk::ApiVersion14};
}

/// @brief Create the Instace Create Info for RAII init in constructor.
///
/// @param context Vulkan Context
/// @param appInfo ApplicationInfo struct
/// @param application_name Name of the game
/// @param extensions List of extensions
/// @param layers List of Vulkan layers
/// @return Create info for Vulkan Instance.
vk::InstanceCreateInfo LSDLVkInstance::makeInstanceCreateInfo(
    vk::raii::Context &context, const vk::ApplicationInfo &appInfo,
    const std::vector<const char *> &extensions,
    const std::vector<const char *> &layers) {

  // Check if the required SDL extensions are supported by the Vulkan
  // implementation.
  auto extensionProperties = context.enumerateInstanceExtensionProperties();
  auto unsupportedPropertyIt = std::ranges::find_if(
      extensions, [&extensionProperties](auto const &requiredExtension) {
        return std::ranges::none_of(
            extensionProperties,
            [requiredExtension](auto const &extensionProperty) {
              return std::string_view(extensionProperty.extensionName) ==
                     requiredExtension;
            });
      });
  if (unsupportedPropertyIt != extensions.end()) {
    throw std::runtime_error("Required extension not supported: " +
                             std::string(*unsupportedPropertyIt));
  }

  // Check if the required layers are supported by the Vulkan implementation.
  auto layerProperties = context.enumerateInstanceLayerProperties();
  auto unsupportedLayerIt = std::ranges::find_if(
      layers, [&layerProperties](auto const &requiredLayer) {
        return std::ranges::none_of(
            layerProperties, [requiredLayer](auto const &layerProperty) {
              return std::string_view(layerProperty.layerName) == requiredLayer;
            });
      });
  if (unsupportedLayerIt != layers.end()) {
    throw std::runtime_error("Required layer not supported: " +
                             std::string(*unsupportedLayerIt));
  }

  return vk::InstanceCreateInfo{
      .pApplicationInfo = &appInfo,
      .enabledLayerCount = static_cast<uint32_t>(layers.size()),
      .ppEnabledLayerNames = layers.data(),
      .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
      .ppEnabledExtensionNames = extensions.data()};
}

} // namespace LVulkan
