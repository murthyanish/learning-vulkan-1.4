module;

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

export module lsdl_resources;

import std;

export struct LSDLSubsystem {
public:
  explicit LSDLSubsystem(SDL_InitFlags flags) : flags(flags) {
    if (!SDL_InitSubSystem(flags)) {
      throw std::runtime_error(std::string("SDL_InitSubSystem failed: ") +
                               SDL_GetError());
    }
  }
  ~LSDLSubsystem() { SDL_QuitSubSystem(flags); }

  LSDLSubsystem(const LSDLSubsystem &) = delete;
  LSDLSubsystem &operator=(const LSDLSubsystem &) = delete;

private:
  SDL_InitFlags flags;
};

export struct LSDLWindow {
public:
  explicit LSDLWindow(
      std::string_view title, int width, int height,
      SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN)) {
    // create blank SDL window for our application
    window = SDL_CreateWindow("Vulkan Engine", // window title
                              1280,            // window width in pixels
                              720,             // window height in pixels
                              flags);
    if (window == nullptr) {
      throw std::runtime_error(std::string("SDL_CreateWindow failed: ") +
                               SDL_GetError());
    }
  }
  ~LSDLWindow() { SDL_DestroyWindow(window); }

  const SDL_Window* get() { return window; }

  LSDLWindow(const LSDLWindow &) = delete;
  LSDLWindow &operator=(const LSDLWindow &) = delete;

private:
  SDL_Window *window;
};
