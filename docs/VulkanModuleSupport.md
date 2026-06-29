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

> [+Note] The Vulkan module and C++ support are separate from the main C
> implementation of Vulkan. As a result, the official Vulkan documentation
> does not make any mention of the Hpp specific terminology or naming.
> I only found out about this later, so many of my early notes here were made
> with the assumption that Vulkan just had bad documentation for everything
> related to Hpp. Which it does, since it doesn't even bother with it.

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

Vulkan-Hpp provides instructions on how to create the module. I only found out
about this long after I finished working on the CMake setup. I might still
look into using this, but for now the project uses the hacked together setup I
made earlier.

Reference:
Vulkan-Hpp module setup: <https://github.com/KhronosGroup/Vulkan-Hpp/blob/main/docs/Usage.md#c20-named-module>

> [+Note] The below info is outdated, skip to the end of this section
>
> The Vulkan module requires module `std`. This is a feature that is only
> supported by C++23. Because of this, I had to switch the standard of the
> library to C++23.
>
> Note: I set the C++ standard to 23 in my project, but tried to build the Vulkan
> module with C++20. This might be why the above error happened. I'll test this
> another time.

## Modules do not export macros

A large part of Vulkan code makes use of macros for various things, however C++
modules don't export macros. This makes it so that you need to use the modern
C++ equivalent of the macros.

The Khronos tutorial makes use of macros and doesn't give info on what the
modern C++ version of these would be. I will have to look this up as I work
through the tutorial.

Some macro functions seem to be replaced with similarly named `vk::XXX` functions:

```cpp
vk::makeVersion(major, minor, patch); // Replaces VK_MAKE_VERSION()
```

## Debug Callbacks

Debug callbacks require macros to function on Windows because it requires
compiler specific attributes. These can only be set via a Macro, so this is
functionality that is not portable to modules as far as I'm aware.

I mostly dev on Linux so this isn't an issue for me, it just resolves to an
empty string anyway. But I want to be safe in case I ever build in another
environment, so I'm including the file `vk_platform.h` which declares these.

This is a minor issue since it isn't really adding much other than the bare
minimum for that platform, so I can use it along with the vulkan module.

Relevant code from `vk_platform.h`:

```c
/* Platform-specific calling convention macros.
 *
 * Platforms should define these so that Vulkan clients call Vulkan commands
 * with the same calling conventions that the Vulkan implementation expects.
 *
 * VKAPI_ATTR - Placed before the return type in function declarations.
 *              Useful for C++11 and GCC/Clang-style function attribute syntax.
 * VKAPI_CALL - Placed after the return type in function declarations.
 *              Useful for MSVC-style calling convention syntax.
 * VKAPI_PTR  - Placed between the '(' and '*' in function pointer types.
 *
 * Function declaration:  VKAPI_ATTR void VKAPI_CALL vkCommand(void);
 * Function pointer type: typedef void (VKAPI_PTR *PFN_vkCommand)(void);
 */
#if defined(_WIN32)
    // On Windows, Vulkan commands use the stdcall convention
    #define VKAPI_ATTR
    #define VKAPI_CALL __stdcall
    #define VKAPI_PTR  VKAPI_CALL
#elif defined(__ANDROID__) && defined(__ARM_ARCH) && __ARM_ARCH < 7
    #error "Vulkan is not supported for the 'armeabi' NDK ABI"
#elif defined(__ANDROID__) && defined(__ARM_ARCH) && __ARM_ARCH >= 7 && defined(__ARM_32BIT_STATE)
    // On Android 32-bit ARM targets, Vulkan functions use the "hardfloat"
    // calling convention, i.e. float parameters are passed in registers. This
    // is true even if the rest of the application passes floats on the stack,
    // as it does by default when compiling for the armeabi-v7a NDK ABI.
    #define VKAPI_ATTR __attribute__((pcs("aapcs-vfp")))
    #define VKAPI_CALL
    #define VKAPI_PTR  VKAPI_ATTR
#else
    // On other platforms, use the default calling convention
    #define VKAPI_ATTR
    #define VKAPI_CALL
    #define VKAPI_PTR
#endif
```

## Other notes

The tutorial uses module `vulkan_hpp`, but the library I am using seems to
only export module `vulkan`. The actual symbols it exports seem to be the same
so I'll continue using this.

## Useful resources

Vulkan-Hpp usage: <https://github.com/KhronosGroup/Vulkan-Hpp/blob/main/docs/Usage.md>
Vulkan-Hpp core concepts: <https://www.khronos.org/assets/uploads/developers/library/2017-vulkan-devu-vancouver/006-VulkanC++Bindings.pdf>
