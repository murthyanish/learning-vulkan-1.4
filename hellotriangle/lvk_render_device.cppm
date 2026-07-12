module;

export module lvk_render_device;

import vulkan;
import lsdl_vk_instance;

namespace LVulkan {

export struct LVkRenderDevice {
public:
  LVkRenderDevice(LSDLVkInstance &instance)
      : physicalDevice(selectPhysicalDevice(instance)) {

        };

  ~LVkRenderDevice() = default;

  LVkRenderDevice(const LVkRenderDevice &) = delete;
  LVkRenderDevice &operator=(const LVkRenderDevice &) = delete;

private:
  vk::raii::PhysicalDevice physicalDevice;
  // vk::raii::Device device;

  static vk::raii::PhysicalDevice &
  selectPhysicalDevice(LSDLVkInstance &instance) {
    auto physicalDevices = instance.getPhysicalDevices();
    if (physicalDevices.empty()) {
      throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    // Device selection:
    std::multimap<int, vk::raii::PhysicalDevice> candidates;
    for (auto &pDevice : physicalDevices) {
      int score = scorePhysicalDevice(pDevice);
      if (score > 0) {
        candidates.insert(std::make_pair(score, pDevice));
      }
    }

    return physicalDevice;
  }
  int scorePhysicalDevice(const vk::raii::PhysicalDevice &pDevice) {
    auto deviceProperties = pDevice.getProperties();
    auto deviceFeatures = pDevice.getFeatures();
    int score = 0;

    // We are focusing on 3D rendering, so we need a geometryShader at bare
    // minimum.
    if (!deviceFeatures.geometryShader) {
      return 0;
    }

    // Prefer a dedicated GPU if present.
    if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
      score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    return 0;
  }
};

} // namespace LVulkan
