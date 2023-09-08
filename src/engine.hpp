//
// Created by Harvey Williams on 20/07/2023.
//

#ifndef TINYRENDER_ENGINE_H
#define TINYRENDER_ENGINE_H

#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <webgpu/webgpu.hpp>

#include "webgpu/wgpu_helpers.hpp"
#include "webgpu/pipelines/Pipeline.hpp"
#include "controllers/Controller.hpp"


// Forward declarations
namespace engine {
    class DepthTexture2D;
}

class Engine {
public:
    Engine(int width, int height);
    ~Engine();

    void launch(); // Launch GLFW window
    int enterMainLoop();

    int onFrame(); // internal call from main event loop

    void onResize();

    void setController(std::shared_ptr<Controller> controller);
    void addPipeline(const std::shared_ptr<Pipeline>& pipeline);

    GLFWwindow *getWindow() { return window; };

    // Some accessors to WGPU API objects that pipelines/primitives need access to do anything
    wgpu::Device wgpuGetDevice() { return device; };
    wgpu::Queue wgpuGetQueue() { return queue; };
    wgpu::TextureFormat wgpuGetSwapChainFormat() { return swapChainFormat; };

    int DISPLAY_WIDTH;
    int DISPLAY_HEIGHT;

    std::shared_ptr<Controller> controller = nullptr;
protected:
    std::shared_ptr<engine::DepthTexture2D> depthTexture = nullptr;

    // GLFW
    GLFWwindow *window;

    // WEBGPU
    wgpu::Instance instance = nullptr;
    wgpu::Surface surface = nullptr;
    wgpu::Adapter adapter = nullptr;
    wgpu::Device device = nullptr;
    wgpu::Queue queue = nullptr;

    wgpu::TextureFormat swapChainFormat = wgpu::TextureFormat::Undefined;
    wgpu::SwapChain swapChain = nullptr;

    // Pipeline objects
    std::vector<std::shared_ptr<Pipeline>> pipelines;
};

void onWindowMouseMove(GLFWwindow* window, double xpos, double ypos);
void onWindowMouseButton(GLFWwindow* window, int button, int action, int mods);
void onWindowScroll(GLFWwindow* window, double xoffset, double yoffset);
void onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif