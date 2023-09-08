//
// Created by Harvey Williams on 03/08/2023.
//

#ifndef TINYGAME_WGPU_HELPERS_CPP_H
#define TINYGAME_WGPU_HELPERS_CPP_H

#include <webgpu/webgpu.hpp>
#include "glfw3webgpu.h"
#include "GLFW/glfw3.h"

using namespace wgpu;


namespace helpers {
    void initializeDevice(Instance &instance, Surface &surface, Adapter &adapter, Device &device);
    void initializeSwapChain(int width, int height, Surface &surface, Adapter &adapter, Device &device, TextureFormat &swapChainFormat, SwapChain &swapChain);
}

#endif //TINYGAME_WGPU_HELPERS_CPP_H
