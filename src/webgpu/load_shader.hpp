//
// Created by Harvey Williams on 20/07/2023.
//

#ifndef TINYGAME_LOAD_SHADER_HPP
#define TINYGAME_LOAD_SHADER_HPP

#include <webgpu/webgpu.hpp>
#include <iostream>
#include <cassert>
#include <filesystem>
#include <fstream>

using namespace wgpu;
namespace fs = std::filesystem;


static ShaderModule loadShaderModule(const fs::path& path, Device device) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return nullptr;
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    std::string shaderSource(size, ' ');
    file.seekg(0);
    file.read(shaderSource.data(), size);

    ShaderModuleWGSLDescriptor shaderCodeDesc{};
    shaderCodeDesc.chain.next = nullptr;
    shaderCodeDesc.chain.sType = SType::ShaderModuleWGSLDescriptor;
    shaderCodeDesc.code = shaderSource.c_str();
    ShaderModuleDescriptor shaderDesc{};
#ifdef WEBGPU_BACKEND_WGPU
    shaderDesc.hintCount = 0;
    shaderDesc.hints = nullptr;
#endif
    shaderDesc.nextInChain = &shaderCodeDesc.chain;
    return device.createShaderModule(shaderDesc);
}

#endif //TINYGAME_LOAD_SHADER_HPP
