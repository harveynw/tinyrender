#pragma once

#include "webgpu/webgpu.hpp"
#include <GLFW/glfw3.h>
#include "glfw3webgpu.h"

#include "primitives/uniforms/UniformStructs.hpp"

/*
 * Context
 * Encapsulates all the fundamental WebGPU resources. 
 */

struct Context {
    wgpu::Instance instance = nullptr;
    wgpu::Surface surface = nullptr;
    wgpu::Adapter adapter = nullptr;
    wgpu::Device device = nullptr;
    wgpu::Queue queue = nullptr;

    void buildSwapChain(int width, int height);

    // Populated by each call to buildSwapChain(...)
    int DISPLAY_WIDTH;
    int DISPLAY_HEIGHT;
    wgpu::TextureFormat swapChainFormat = wgpu::TextureFormat::Undefined;
    wgpu::SwapChain swapChain = nullptr;

    ~Context();

    void print();
};

std::shared_ptr<Context> buildNewContext(GLFWwindow *window, int width, int height);