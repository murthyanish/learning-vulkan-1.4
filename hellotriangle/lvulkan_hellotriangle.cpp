#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>

// #include "vulkan_resources.h"

import std;
import vulkan;
import lsdl_resources;

class HelloTriangleApplication {
  /// Resources
  // NOTE: members are declared in initialization order.
  // Destruction will occur in reverse order. Do not modify the order unless
  // you are certain of the dependencies.

  // Init SDL video subsystem
  LSDLSubsystem sdlVideo { SDL_INIT_VIDEO };
  // Init SDL window
  LSDLWindow sdlWindow { "HelloTriangle", 1280, 720 };

public:
  void run() {
    initVulkan();
    mainLoop();
    cleanup();
  }

private:
  void initVulkan() {
    Uint32 count;
  }

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

  void cleanup() {
  }
};

int main() {
  try {
    HelloTriangleApplication app;
    app.run();
  } catch (const std::exception &e) {
    SDL_LogError(0, "%s", e.what());
    return 1;
  }

  return 0;
}
