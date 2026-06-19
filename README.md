# learning-vulkan-1.4

Trying out the official Khronos Vulkan Tutorial to learn the new features in
Vulkan 1.4

I'm using SDL instead of GLFW, since I already have familiarity with using GLFW
for Vulkan. This is a good opportunity to learn SDL.

**Reminder to self:**
Until first draw, Wayland doesn't display a window. This is a problem for
testing before surface is ready, so just set the below:

`export SDL_VIDEO_DRIVER=x11`
