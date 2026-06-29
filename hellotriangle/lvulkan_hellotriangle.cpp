#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>

// #include "vulkan_resources.h"

import std;
import vulkan;
import lsdl_resources;
import lvulkan_resources;

// Debug logging
#ifdef NDEBUG
constexpr bool enableVerboseLogging = false;
#else
constexpr bool enableVerboseLogging = true;
#endif

class HelloTriangleApplication {
  /// Resources
  // NOTE: members are declared in initialization order.
  // Destruction will occur in reverse order. Do not modify the order unless
  // you are certain of the dependencies.

  // SDL video subsystem
  LSDLSubsystem sdlVideo;
  // SDL window
  LSDLWindow sdlWindow;
  // Vulkan context and instance
  LSDLVulkanInstance sdlVulkanInstance;
  // Vulkan debug message stuff
  LVulkanDebugMessenger vulkanDebugMessenger;

public:
  HelloTriangleApplication()
      : sdlVideo(SDL_INIT_VIDEO),
        sdlWindow(sdlVideo, "HelloTriangle", 1280, 720),
        sdlVulkanInstance(sdlVideo, sdlWindow, "HelloTriangle"),
        vulkanDebugMessenger(sdlVulkanInstance) {
    // Each hint describes when it should be set, this one should be set before
    // SDL is initialized.
    SDL_SetHint(SDL_HINT_APP_NAME, "HelloTriangleApplication");
    if (enableVerboseLogging) {
      SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION,
                         SDL_LOG_PRIORITY_VERBOSE);
    }
  }

  void run() { mainLoop(); }

private:
  void mainLoop() {
    SDL_Event e;
    bool quit = false;

    while (!quit) {
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
          quit = true;
        }
      }
    }
  }
};

int main() {
  try {
    HelloTriangleApplication app;
    app.run();
  } catch (const std::exception &e) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", e.what());
    return 1;
  }

  return 0;
}
