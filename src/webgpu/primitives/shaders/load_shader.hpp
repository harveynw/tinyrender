#pragma once

#include <webgpu/webgpu.hpp>
#include <iostream>
#include <cassert>
#include <fstream>
#include <map>

using namespace wgpu;

const std::map<std::string, std::string> SHADER_SOURCES = {
    {
        "basic_mvp_color", 
        #include "shaders/basic_mvp_color.wgsl"
    },
    {
        "basic_mvp_texture",
        #include "shaders/basic_mvp_texture.wgsl"
    },
    {
        "voxels",
        #include "shaders/voxels.wgsl"
    },
    {
        "waves",
        #include "shaders/waves.wgsl"
    }
};

ShaderModule loadShaderModule(std::string name, Device device);
