# Vulkan Module Support

C++ Modules in general seem to not be very intuitive or well supported in
Linux. While Visual Studio natively handles support for it, it is a nightmare
to set this up on Linux no matter what compiler you use.

This isn't purely because of compiler or build support, but a combination of
not having easy to find documentation or examples that are well maintained. A
majority of the documentation I did find either had concepts explained without
examples or broken examples and outdated info.

This is something I had struggled with on two separate occasions in the past
leading me to abandon the cause each time. This time I pushed through to
finally get this working.

Since C++ modules seem to be the future of C++, I decided that it would
eventually be worth it.

I elected to use clang++ and CMake since this is the combination that is most
recommended.

## Modules with CMake

CMake can natively handle module imports and creation, but it only has
experimental std support. This is a problem when the Vulkan module (seemingly)
needs the std module to be compiled.

This required me adding the following code:
```
# Enable C++ module dependency scanning only if C++ 23 module is enabled
if(ENABLE_CPP23_MODULE)
  set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
  # Explicitly tell CMake this target consumes standard library modules
  set(CMAKE_CXX_MODULE_STD ON)
  # Tell CMake to scan source files for module dependency mapping
  set(CMAKE_CXX_SCAN_FOR_MODULES ON)
endif()
```

This seems to not be well documented, because the CMake documentation website
doesn't include examples and doesn't give specific details about the commands
needed.

The experimental flag's guid specific to the CMake version. It isn't documented
on the website and I had to find it by searching it online. The docs mention it
is in a specific file, but doesn't tell us where we can find this file.

## Vulkan Module

The Khronos tutorial (website) doesn't properly document and use Vulkan modules.
This is an issue that required me to find fixes to problems that aren't covered
in the tutorial.

### Building the Vulkan Module

The Vulkan module requires module `std`. This is a feature that is only
supported by C++23. Because of this, I had to switch the standard of the
library to C++23.

Note: I set the C++ standard to 23 in my project, but tried to build the Vulkan
module with C++20. This might be why the above error happened. I'll test this
another time.

## Modules do not export macros

A large part of Vulkan code makes use of macros for various things, however C++
modules don't export macros. This makes it so that you need to use the modern
C++ equivalent of the macros.

The Khronos tutorial makes use of macros and doesn't give info on what the
modern C++ version of these would be. I will have to look this up as I work
through the tutorial.
