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
flags, else we might destroy

## Using RAII for init in class

Classes init their member functions in order of declaration. A fun effect of
this is that you can declare and instantiate your RAII resources by declaring
them in order in the class definition. When being destroyed, the class will
always destroy resources in the inverse order that they are init.

Not the most readable, but it works.

## Notes

- This is meant to be a thin wrapper around objects, so typically we should
  avoid bloating them too much. That said, adding functions to interact with
  the resource is fine.
- Typically we would want the resource itself to still be accessible
  instead of making it private. Usually we can't cover every case for the
  resource, so making it private limits its usability without much benefit.
- Polymorphism is alright, but adding virtual functions will add a vtable,
  adding indirection, so maybe avoid it unless there's a good reason.
