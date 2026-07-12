# Modern C++

I'm going to put notes around modern C++ features I learn for future reference.

## Modules

Modules replace preprocessor-based `#include` mechanism for sharing code
between source files. Instead, modules treat components as a single compiled
unit with a clearly defined public interface.

### The Pluses

Earlier, `#include` would literally just copy paste the entire header file in
its place. This meant that lots of headers and recursive header calls would
bloat the compiled file massively. This also led to lots of related issues like
cycles, header dependencies that break the code when reordered, and so on.

This means the old interface/implementation separation is no longer used, and
even just having all the code in one module file would be fine.

Modules are also only compiled once, so compilation is way faster. It creates
a bytecode representation called a "Built Module Interface".

With modules, you need to explicitly specify what is shared with the `export`
keyword. This means you can have any amount of code in the module and it won't
pollute the code outside the module.

### The Negatives

Everything related to build is a nightmare. It's getting there, but it isn't
remotely ready yet for general use. I can see why it isn't taught in C++
classes and tutorials. And why I've rarely seen it out in the wild. Even the
Khronos Vulkan Tutorial, which specifically mentions modules and has a module
build, just does not work out of the box.

> [!NOTE]
> Refer to `VulkanModuleSupport.md` for all the build details for modules. It's
> a mess, and I needed a separate document for it. Thankfully once the build is
> set up correctly, it just works. I wonder if I can contribute to the project
> to help the next person.

### Module Concepts

#### Module Partition

We can split modules across multiple files using module partition. This isn't
the same as a header/implementation split, the implementation is still in the
same file. Instead, it's closer to a public/private split for the exported
code.

The module is split into two parts:

- __Implementation partition__: This is where the actual code lies, and any code in
  an implementation partition can access all the other partitions of the same
  module.

  ```cpp
  module;

  // #includes
  ...

  module lsdl_vk_instance:instance;

  // imports
  ...

  export namespace {
    // All the code that needs to be public
    ...
  }
  ```

  ```cpp
  module;

  // #includes
  ...

  export module lsdl_vk_instance:debug_messenger;

  // imports
  ...

  // importing implementations under the same module
  import :instance;     // needs LSDLVkInstance for the constructor param

  namespace LVulkan {
    // private functions
    void exampleFunction() {}

    // public class
    export struct LVkDebugMessenger {
      ...
    };
  }
  ```
- __Interface partition__: This is closer to an access controller that combines
  all the module implementations and exports only the implementations that are
  meant to be publicly accessible.

  ```cpp
  export module lsdl_vk_instance;

  export import :instance;
  export import :debug_messenger;
  ```

Separating the code this way has multiple benefits:

- __Control visibility better__: The module implementations can be freely imported
  between the files regardless of export. Then the interface file can control
  which implementation files are exported. This means if we put private code in
  an implementation file and do not export it in the interface file, we can be
  100% certain that everything defined in the file will not leak.
- __Build parallelism__: Each of the module interface files can be compiled in
  parallel. Even if the module is pretty large, we can split it easily to make
  it easier to organize while also making it faster. This also means we can
  modify any implementation file and we don't need to worry about rebuilding
  the entire module for that small change.

## String Views

A string view is a lightweight, non-owning reference to a contiguous sequence of
characters.

String views just store a reference to the original string, and the length of
the string.

It's very convenient for functions because it can automatically convert strings
to string views whether it is a regular string, c style strings or other string
views. Convert here means it can accept them as inputs, it does not allocate
new memory for the data.

It also provides comparison operators such as `==`, `<` and `>`. This makes it
so the `strcmp` call is not needed.

> [!IMPORTANT]
> String views can outlive the scope of the source string, which leads to
> undefined behavior. Don't use string views if there's any chance of the
> source string being deleted.

## Compiler Hints

Compiler hints are standardised now, so no more commands with
`__builtin_expect`. Instead, it's in the format `[[xyz]]`.

- `[[nodiscard]]`: This makes the compiler check if the value being returned
  is used or not. This helps greatly in cases where the return of a function is
  costly, and the function doesn't have other side-effects. We typically don't
  want to waste the time calling the function if we don't even use it.
- `[[likely]]`/`[[unlikely]]`:
  > [!IMPORTANT]
  > Don't use this unless you've already run profiling.
  It tells the compiler to arrange the compiled bytecode so the
  specified likely branch is contiguous with the rest of the code, and the
  unlikely branch is moved to a cold region.
