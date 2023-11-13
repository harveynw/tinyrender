#pragma once

#include <webgpu/webgpu.hpp>
#include <iostream>
#include <cassert>
#include <fstream>

using namespace wgpu;

const std::string basic_mvp_color =
#include "shaders/basic_mvp_color.wgsl"
;

const std::string basic_mvp_texture =
#include "shaders/basic_mvp_texture.wgsl"
;

const std::string voxels =
#include "shaders/voxels.wgsl"
;

const std::string waves =
#include "shaders/waves.wgsl"
;


ShaderModule loadShaderModule(std::string name, Device device);
