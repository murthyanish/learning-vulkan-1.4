module;

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_vulkan.h>
#include <stdexcept>

export module lvulkan_resources;

import vulkan;
import std;

class LSDLVulkanInstance {
public:
  LSDLVulkanInstance(std::string_view application_name,
                     bool enable_validation_layers = false) {
    vk::ApplicationInfo appInfo{.pApplicationName = application_name.data(),
                                .applicationVersion = vk::makeVersion(1, 0, 0),
                                .pEngineName = "No Engine",
                                .engineVersion = vk::makeVersion(1, 0, 0),
                                .apiVersion = vk::ApiVersion14};

    // Get the required extensions from SDL and set count to the number of
    // extensions
    uint32_t extensionCount = 0;
    const char *const *sdlExtensions =
        SDL_Vulkan_GetInstanceExtensions(&extensionCount);

    if (sdlExtensions == NULL) {
      throw std::runtime_error(
          std::string("SDL_Vulkan_GetInstanceExtensions failed: ") +
          SDL_GetError());
    }

    // if (enable_validation_layers) {
    //   // If validation layers are enabled, add the debug utils extension
    //   const char **extensions = (const char**)malloc(sizeof(const char *) *
    //   (extensionCount + 1)); memcpy(extensions, sdlExtensions, sizeof(const
    //   char *) * extensionCount); extensions[extensionCount] =
    //   VK_EXT_DEBUG_UTILS_EXTENSION_NAME; extensionCount += 1;
    // }

    // Check if the required SDL extensions are supported by the Vulkan
    // implementation.
    auto extensionProperties = context.enumerateInstanceExtensionProperties();
    for (uint32_t i = 0; i < extensionCount; ++i) {
      if (std::ranges::none_of(
              extensionProperties,
              [sdlExtension = sdlExtensions[i]](auto const &extensionProperty) {
                return strcmp(extensionProperty.extensionName, sdlExtension) ==
                       0;
              })) {
        throw std::runtime_error("Required SDL extension not supported: " +
                                 std::string(sdlExtensions[i]));
      }
    }

    vk::InstanceCreateInfo createInfo{
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = extensionCount,
        .ppEnabledExtensionNames = sdlExtensions};

    instance = vk::raii::Instance(context, createInfo);
  }

  const vk::raii::Instance &get() { return instance; }
  const vk::raii::Context &getContext() { return context; }

  ~LSDLVulkanInstance() = default;

private:
  vk::raii::Context context;
  vk::raii::Instance instance = nullptr;
};
