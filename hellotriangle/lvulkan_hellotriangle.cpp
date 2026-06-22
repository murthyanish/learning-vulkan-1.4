#include <SDL3/SDL_log.h>
#include <SDL3/SDL_vulkan.h>

#include <stdexcept>
#include <cstdlib>

import std;

class HelloTriangleApplication {
public:
    void run() {
        throw std::runtime_error("Testing errors.");
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    void initVulkan() {

    }

    void mainLoop() {

    }

    void cleanup() {

    }
};

int main() {
    HelloTriangleApplication app;
    std::cout << "Hello modular world\n";

    try {
        app.run();
    } catch (const std::exception& e) {
        SDL_LogError(0, "%s", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
