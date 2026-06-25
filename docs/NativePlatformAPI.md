# Native Platform API

Native platform API are how we interact with the OS without using external
libs. This differs between OS, so writing an app on Linux is different from
writing the same app on Windows or macOS, etc.

To make life easier, we usually use a middle man to interact with these, like
GLFW or SDL.

## Common Native Platform API:

- Windows:
  - Win32: Legacy Win programming interface.
  - WinRT: Modern API designed for sandboxed cross-CPU architecture apps.
- Linux:
  - Qt: For KDE.
  - GTK: For Gnome.
  - Linux Kernel / POSIX / Virtual Filesystem: Everything non-GUI.
- macOS:
  - Metal and POSIX.
