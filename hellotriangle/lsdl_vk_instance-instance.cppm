module;
#include <string_view>
export module lsdl_vk_instance:instance;

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
                          bool enable_validation_layers = false);

  [[nodiscard]] std::vector<vk::raii::PhysicalDevice>
  getPhysicalDevices() const;

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
  static std::vector<const char *> getRequiredInstanceExtensions();

  /// @brief Get the requires layers for the engine.
  ///
  /// @return Vector of layer names.
  static std::vector<const char *> getRequiredLayers();

  /// @brief Make an ApplicationInfo struct.
  ///
  /// @param application_name Application/Game name
  /// @return ApplicationInfo struct.
  static vk::ApplicationInfo
  makeApplicationInfo(std::string_view application_name);

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
                         const std::vector<const char *> &layers);
};

} // namespace LVulkan
