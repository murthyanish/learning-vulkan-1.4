module lsdl_vk_instance:validation;

import std;

namespace LVulkan {

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
constexpr bool enableValidationCallbacks = false;
#else
constexpr bool enableValidationLayers = true;
constexpr bool enableValidationCallbacks = true;
#endif

/// @brief Vulkan validation layer we will be using.
const std::vector<char const *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

} // namespace LVulkan
