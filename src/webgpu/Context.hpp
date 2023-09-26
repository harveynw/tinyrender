#pragma once

#include "webgpu/webgpu.hpp"
#include <GLFW/glfw3.h>
#include "glfw3webgpu.h"

#include "primitives/buffers/uniforms/UniformStructs.hpp"

/*
 * Context
 * Encapsulates a tightly coupled instance of WebGPU resources. Can be passed around to provide easy access to them.
 */

struct Context {
    int DISPLAY_WIDTH = 0;
    int DISPLAY_HEIGHT = 0;

    wgpu::Instance instance = nullptr;
    wgpu::Surface surface = nullptr;
    wgpu::Adapter adapter = nullptr;
    wgpu::Device device = nullptr;
    wgpu::Queue queue = nullptr;

    wgpu::TextureFormat swapChainFormat = wgpu::TextureFormat::Undefined;
    wgpu::SwapChain swapChain = nullptr;

    ~Context();

    void buildSwapChain();
    void print();
};

std::shared_ptr<Context> buildNewContext(GLFWwindow *window, int, int);