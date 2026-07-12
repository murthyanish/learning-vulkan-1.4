module;

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_vulkan.h>
#include <string_view>

export module lsdl_vk_instance:instance;

import :validation;

import vulkan;
import std;
import lsdl_resources;

export namespace LVulkan {

/// @class LSDLVkInstance
/// @brief Vulkan Instance
///
/// NOTE: The constructor used here is kinda wonky, and I might just remove it
/// later to be safe. This needs to be this way to work if I'm using this route
/// though.
///
/// I've used a function call to create an argument for another function, which
/// finally returns the data for the argument of instance. The reason for this
/// is that `InstanceCreateInfo` needs to have a reference to an object of type
/// `ApplicationInfo`. If I have this created inside `makeApplicationInfo`, the
/// object will go out of scope at the end of the function, and Vulkan will
/// receive a pointer to garbage.
///
/// By doing this, we move scope of the application info out of the
/// `makeApplicationInfo` function, so it lives long enough to construct the
/// instance.
///
/// The whole reason for this is because I'm experimenting with RAII and having
/// resources created on initialization vs assignment.
///
/// Insert "Your Scientists Were So Preoccupied With Whether Or Not They Could,
/// They Didn’t Stop To Think If They Should" meme here.
struct LSDLVkInstance {
public:
  explicit LSDLVkInstance(LSDLSubsystem &sdlVideo, // To enforce dependency
                          LSDLWindow &sdlWindow,   // To enforce dependency
                          std::string_view application_name,
                          bool enable_validation_layers = false)
      : context(),
        instance(context,
                 makeInstanceCreateInfo(
                     context, makeApplicationInfo(application_name),
                     getRequiredInstanceExtensions(), getRequiredLayers())) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Vulkan Instance created!");
  }

  [[nodiscard]] std::vector<vk::raii::PhysicalDevice>
  getPhysicalDevices() const {
    return instance.enumeratePhysicalDevices();
  }

  const vk::raii::Instance &get() const { return instance; }
  const vk::raii::Context &getContext() const { return context; }

  ~LSDLVkInstance() = default;
  LSDLVkInstance(const LSDLVkInstance &) = delete;
  LSDLVkInstance &operator=(const LSDLVkInstance &) = delete;
  LSDLVkInstance(LSDLVkInstance &&) = default;
  LSDLVkInstance &operator=(LSDLVkInstance &&) = default;

private:
  vk::raii::Context context;
  vk::raii::Instance instance;

  /// @brief Get the required extensions from SDL.
  ///
  /// @return Vector of Vulkan extension names.
  static std::vector<const char *> getRequiredInstanceExtensions() {
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
  static std::vector<const char *> getRequiredLayers() {
    return enableValidationLayers
               ? std::vector<const char *>(validationLayers.begin(),
                                           validationLayers.end())
               : std::vector<const char *>{};
  }

  /// @brief Make an ApplicationInfo struct.
  ///
  /// @param application_name Application/Game name
  /// @return ApplicationInfo struct.
  static vk::ApplicationInfo
  makeApplicationInfo(std::string_view application_name) {
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
  static vk::InstanceCreateInfo
  makeInstanceCreateInfo(vk::raii::Context &context,
                         const vk::ApplicationInfo &appInfo,
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
                return std::string_view(layerProperty.layerName) ==
                       requiredLayer;
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
};

} // namespace LVulkan
