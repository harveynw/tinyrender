//
// Created by Harvey Williams on 20/07/2023.
//

#ifndef TINYGAME_LOAD_SHADER_HPP
#define TINYGAME_LOAD_SHADER_HPP

#include <webgpu/webgpu.hpp>
#include <iostream>
#include <cassert>
#include <fstream>

using namespace wgpu;

ShaderModule loadShaderModule(std::string path, Device device);

#endif //TINYGAME_LOAD_SHADER_HPP
