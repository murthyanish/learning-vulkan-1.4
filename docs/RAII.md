# RAII: Resource Aquisition is Initialization

RAII is the practice of tying resource lifecycles to the resource lifetime.

Basically, any object you need to allocate or which has its own cleanup needs
to be handled as an RAII object. The simplest implementation is to make such
resources a class object where the constructor does initialization and
destructor does cleanup.

eg. `SDL_InitSubsystem`:
SDL is a C api, so it lacks features like RAII. To make it safer, we can use
an SDLSubsystem class which can hold the subsystem resource and handle its
lifecycle. It should help make it easier to track when things go wrong too
since I can tie logging into it to know exactly when the resource is modified
instead of guessing.

```cpp
struct SDLSubsystem {
public:
  explicit SDLSubsystem(SDL_InitFlags flags) : flags(flags) {
    if (!SDL_InitSubSystem(flags)) {
      throw std::runtime_error(std::string("SDL init failed: ") +
                               SDL_GetError());
      return;
    }
  }
  ~SDLSubsystem() { SDL_QuitSubSystem(flags); }

  SDLSubsystem(const SDLSubsystem &) = delete;
  SDLSubsystem &operator=(const SDLSubsystem &) = delete;

private:
  SDL_InitFlags flags;
};
```

Here, I use flags as a private member because we want to maintain the value of
flags, else we might destroy.

> [!NOTE]
> I've decided to use `struct` instead of `class` for RAII objects to make it
> easier to differentiate between these and old style classes for me.

## Using RAII for init in class

Classes init their member functions in order of declaration. A fun effect of
this is that you can declare and instantiate your RAII resources by declaring
them in order in the class definition. When being destroyed, the class will
always destroy resources in the inverse order that they are init.

Not the most readable, but it works.

### Enforcing dependencies

When initializing classes, to ensure that all the resources are correctly
ordered, we can add resources as required arguments to the subclasses or
functions used in the constructor.

```cpp
explicit LSDLVkInstance(LSDLSubsystem &sdlVideo, // To enforce dependency
                          LSDLWindow &sdlWindow,   // To enforce dependency
                          std::string_view application_name,
                          bool enable_validation_layers = false)
      : context(),
        instance(...) {
  ...
}
```

### Prevent unintended initializations

Older style code typically sets its data members to `nullptr` or similar. This
leads to resources being initialized unintentionally, and then modifying the
value instead of directly initializing the resource on object creation.

To prevent this, it is advised to move initialization logic to static functions,
and have the constructor initialization list call the functions as the required
arguments for the data members.

```cpp
LVkRenderDevice(LSDLVkInstance &instance)
    : physicalDevice(selectPhysicalDevice(instance)) {
  ...
}

static vk::raii::PhysicalDevice &selectPhysicalDevice(
    LSDLVkInstance &instance) {
  ...
}
```

## Notes

- This is meant to be a thin wrapper around objects, so typically we should
  avoid bloating them too much. That said, adding functions to interact with
  the resource is fine.
- Typically we would want the resource itself to still be accessible
  instead of making it private. Usually we can't cover every case for the
  resource, so making it private limits its usability without much benefit.
- Polymorphism is alright, but adding virtual functions will add a vtable,
  adding indirection, so maybe avoid it unless there's a good reason.
