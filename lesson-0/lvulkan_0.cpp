#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

int main(void) {
  SDL_Log("%s", "Hello World!");

  // Initialize app
	if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL initialization failed: %s", SDL_GetError());
    return -1;
  }

  // Load Vulkan driver
  if (!SDL_Vulkan_LoadLibrary(nullptr)) {
    SDL_Log("Could not load Vulkan library! SDL error: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Create window
  SDL_PropertiesID window_props {SDL_CreateProperties()};
  SDL_SetNumberProperty(window_props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, 800);
  SDL_SetNumberProperty(window_props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, 800);
  SDL_SetBooleanProperty(window_props, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, false);
  SDL_SetBooleanProperty(window_props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, false);
  SDL_SetBooleanProperty(window_props, SDL_PROP_WINDOW_CREATE_VULKAN_BOOLEAN, true);
  SDL_SetStringProperty(window_props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "Vulkan Triangle");
  SDL_Window* window = SDL_CreateWindowWithProperties(window_props);
  if (window == nullptr) {
    SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  uint32_t sdl_extension_count;
  const char* const* sdl_extension_names = SDL_Vulkan_GetInstanceExtensions(&sdl_extension_count);

  SDL_Log("%d  extensions supported\n", sdl_extension_count);

  glm::mat4 matrix;
  glm::vec4 vec;
  auto test = matrix * vec;

  SDL_Event e;
  bool bQuit = false;

  // main loop
  while (!bQuit) {
    // Handle events on queue
    while (SDL_PollEvent(&e) != 0) {
      // close the window when user clicks the X button or alt-f4s
      if (e.type == SDL_EVENT_QUIT)
        bQuit = true;
    }
  }

  SDL_DestroyWindow(window);
  SDL_Vulkan_UnloadLibrary();

  SDL_Quit();

  return 0;
}
